/*
 *  @(#)DataCppLoader.java  last: 13.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.load.compile.ParseException;
import com.lasgis.arduino.eeprom.load.compile.RomDataWrapper;
import com.lasgis.arduino.eeprom.load.compile.TokenParser;
import com.lasgis.arduino.eeprom.load.compile.TokenWrapper;
import com.lasgis.arduino.eeprom.memory.BatchMemory;
import com.lasgis.arduino.eeprom.memory.MemoryRoms;
import com.lasgis.arduino.eeprom.memory.RomARRAY;
import com.lasgis.arduino.eeprom.memory.RomCHAR;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.arduino.eeprom.memory.RomEMPTY;
import com.lasgis.arduino.eeprom.memory.RomFLOAT;
import com.lasgis.arduino.eeprom.memory.RomINT16;
import com.lasgis.arduino.eeprom.memory.RomINT32;
import com.lasgis.arduino.eeprom.memory.RomINT8;
import com.lasgis.arduino.eeprom.memory.RomOBJECT;
import com.lasgis.arduino.eeprom.memory.RomSTRING;
import com.lasgis.util.Util;
import org.apache.commons.lang3.StringEscapeUtils;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import static com.lasgis.arduino.eeprom.load.compile.TokenParser.KeywordType.BATCH_MEMORY;
import static com.lasgis.arduino.eeprom.load.compile.TokenParser.KeywordType.MEMORY_ROMS;
import static java.util.Objects.nonNull;

/**
 * Загрузчик из *.data файла.
 * Это с-подобная структура.
 *
 * @author Vladimir Laskin
 * @since 30.05.2018
 */
class DataCppLoader extends TokenParser {

    public DataCppLoader() {
        super();
    }

    static MemoryRoms load(final File file) throws ParseException {
        final DataCppLoader loader = new DataCppLoader();
        return loader.loadFile(file);
    }

    private MemoryRoms loadFile(final File file) throws ParseException {
        setProgramCode(Util.loadStringFromFile(file));
        return parse();
    }

    public MemoryRoms parse() throws ParseException {
        final int beg = 0;
        final int end = sb.length() - 1;
        int i = beg;
        Token token;
        List<RomData> romDataList = new ArrayList<>();
        final List<BatchMemory> batchMemoryList = new ArrayList<>();
        final MemoryRoms memoryRoms = MemoryRoms.of("rom_memory", batchMemoryList);
        BatchMemory batchMemory = BatchMemory.of();
        batchMemory.setPrefix("batch");
        batchMemory.setDevice((byte) 0x00);
        batchMemory.setAddress(0x000);
        batchMemory.setRomDataList(romDataList);
        batchMemoryList.add(batchMemory);
        do {
            final TokenWrapper<MemoryRoms> memoryWrap = getMemoryRoms(i, end);
            if (nonNull(memoryWrap)) {
                final MemoryRoms memoryRomsWrap = memoryWrap.getData();
                memoryRoms.setHeaderFilename(memoryRomsWrap.getHeaderFilename());
                token = memoryWrap.getToken();
                i = token.beg;
            }
            final TokenWrapper<BatchMemory> batchWrap = getBatchMemory(i, end);
            if (nonNull(batchWrap)) {
                final BatchMemory bm = batchMemoryList.get(batchMemoryList.size() - 1);
                if (bm.getRomDataList().isEmpty()) {
                    batchMemoryList.remove(bm);
                }
                batchMemory = batchWrap.getData();
                romDataList = batchMemory.getRomDataList();
                batchMemoryList.add(batchMemory);
                token = batchWrap.getToken();
                i = token.beg;
            }
            final RomDataWrapper wrap = getRomData(i, end, null);
            final RomData data = wrap.getData();
            token = wrap.getToken();
            i = token.beg;
            if (!(data instanceof RomEMPTY)) {
                romDataList.add(data);
            }
        } while (!token.is(TokenType.end));
        return memoryRoms;
    }

    private TokenWrapper<MemoryRoms> getMemoryRoms(
        final int beg, final int end
    ) throws ParseException {
        Token token = nextToken(beg, end).SkipComment(end);
        if (token.is(TokenType.keyword, MEMORY_ROMS.getName())) {
            token = token.next(end).SkipComment(end).assertion(TokenType.delimit, ":");
            token = token.next(end).SkipComment(end).assertion(TokenType.block, "{");
            final MemoryRoms memoryRoms = extractMemoryRoms(token);
            token = token.next(end);
            token = token.SkipComment(end);
            token = token.assertion(TokenType.delimit, ";");
            token = token.next(end);
            token = token.SkipComment(end);
            return TokenWrapper.of(memoryRoms, token);
        }
        return null;
    }

    private TokenWrapper<BatchMemory> getBatchMemory(
        final int beg, final int end
    ) throws ParseException {
        Token token = nextToken(beg, end).SkipComment(end);
        if (token.is(TokenType.keyword, BATCH_MEMORY.getName())) {
            token = token.next(end).SkipComment(end).assertion(TokenType.delimit, ":");
            token = token.next(end).SkipComment(end).assertion(TokenType.block, "{");
            final BatchMemory batch = extractBatchMemory(token);
            token = token.next(end);
            token = token.SkipComment(end);
            token = token.assertion(TokenType.delimit, ";");
            token = token.next(end);
            token = token.SkipComment(end);
            return TokenWrapper.of(batch, token);
        }
        return null;
    }

    private MemoryRoms extractMemoryRoms(final Token blockToken) throws ParseException {
        final int beg = blockToken.beg + 1;
        final int end = blockToken.end - 1;
        int i = beg;
        Token token;
        final MemoryRoms memoryRoms = MemoryRoms.of();
        do {
            token = nextToken(i, end).SkipComment(end).assertion(TokenType.name, "headerFilename");
            token = token.next(end).SkipComment(end).assertion(TokenType.delimit, ":");
            token = token.next(end).SkipComment(end).assertion(TokenType.string);
            final Token[] tokens = {token};
            memoryRoms.setHeaderFilename(extractString(tokens, end));
            token = tokens[0].next(end).SkipComment(end);
        } while (!token.is(TokenType.end));
        return memoryRoms;
    }

    private BatchMemory extractBatchMemory(final Token blockToken) throws ParseException {
        final int beg = blockToken.beg + 1;
        final int end = blockToken.end - 1;
        int i = beg;
        Token token;
        final BatchMemory batch = BatchMemory.of();
        do {
            token = nextToken(i, end).SkipComment(end).assertion(TokenType.name, "device", "prefix", "address");
            final String key = token.getString();
            token = token.next(end).SkipComment(end).assertion(TokenType.delimit, ":");
            token = token.next(end).SkipComment(end);
            final Token[] tokens = {token};
            switch (key) {
                case "device":
                    batch.setDevice((byte) extractInteger(tokens, end));
                    break;
                case "prefix":
                    batch.setPrefix(extractString(tokens, end));
                    break;
                case "address":
                    batch.setAddress(extractInteger(tokens, end));
                    break;
                default:
                    throw new ParseException(token, "Непонятный ключ");
            }
            token = tokens[0].next(end)
                .SkipComment(end)
                .Skip(TokenType.delimit, end)
                .SkipComment(end);
            i = token.beg;
        } while (!token.is(TokenType.end));
        return batch;
    }

    private RomDataWrapper getRomData(
        final int beg, final int end,
        final KeywordType expectedType
    ) throws ParseException {
        Token token = nextToken(beg, end).SkipComment(end);
        if (token.is(TokenType.end)) {
            return RomDataWrapper.of(RomEMPTY.of(), token);
        }
        /* тип основного RomData */
        KeywordType type = expectedType;
        /* дополнительный тип для RomARRAY */
        KeywordType arrayType = null;
        /* наименование основного RomData */
        String name = null;

        /* тип основного RomData (для RomOBJECT и RomARRAY пропускается) */
        if (token.is(TokenType.keyword)) {
            type = KeywordType.of(token.getString());
            token = token.next(end).SkipComment(end);
        }
        /* наименование основного RomData */
        if (token.is(TokenType.name)) {
            name = token.getString();
            token = token.next(end).SkipComment(end);
        }
        /* дополнительный тип вложенных Rom для RomARRAY */
        if (token.is(TokenType.keyword)) {
            arrayType = KeywordType.of(token.getString());
            token = token.next(end).SkipComment(end);
        }
        /* окончание описания и переход на заполнение значения */
        if (token.is(TokenType.delimit, ":")) {
            token = token.next(end);
        }
        RomData data = RomEMPTY.of();
        if (type != null) {
            final Token[] tokens = {token};
            switch (type) {
                case CHAR: {
                    final char chr = extractChar(tokens, end);
                    data = RomCHAR.of(name, null, chr);
                }
                break;
                case INT8: {
                    final int value = extractInteger(tokens, end);
                    data = RomINT8.of(name, null, value);
                }
                break;
                case INT16: {
                    final int value = extractInteger(tokens, end);
                    data = RomINT16.of(name, null, value);
                }
                break;
                case INT32: {
                    final int value = extractInteger(tokens, end);
                    data = RomINT32.of(name, null, value);
                }
                break;
                case FLOAT: {
                    final double value = extractDouble(tokens, end);
                    data = RomFLOAT.of(name, null, value);
                }
                break;
                case STRING: {
                    final String value = extractString(tokens, end);
                    data = (value == null) ? RomEMPTY.of(name) : RomSTRING.of(name, null, value);
                }
                break;
                case OBJECT: {
                    token.assertion(TokenType.block, "{");
                    data = extractRomDataObject(name, token);
                }
                break;
                case ARRAY: {
                    token.assertion(TokenType.block, "[");
                    data = extractRomDataArray(name, token, arrayType);
                }
                break;
                default:
                    throw new ParseException(token, "Need to parse type: \"" + type + "\"");
            }
            token = tokens[0];
        } else if (token.is(TokenType.block, "{")) {
            data = extractRomDataObject(name, token);
        } else if (token.is(TokenType.block, "[")) {
            data = extractRomDataArray(name, token, arrayType);
        } else {
            throw new ParseException(token, "Expected type of Rom Data.");
        }
        token = token.next(end);
        token = token.SkipComment(end);
        token = token.assertion(TokenType.delimit, ";", ",");
        token = token.next(end);
        token = token.SkipComment(end);
        return RomDataWrapper.of(data, token);
    }

    private RomOBJECT extractRomDataObject(final String name, final Token arrToken) throws ParseException {
        final int beg = arrToken.beg + 1;
        final int end = arrToken.end - 1;
        int i = beg;
        Token token;
        final RomOBJECT object = RomOBJECT.of(name, null);
        do {
            final RomDataWrapper wrap = getRomData(i, end, null);
            final RomData data = wrap.getData();
            token = wrap.getToken();
            i = token.beg;
            if (!(data instanceof RomEMPTY)) {
                object.add(data);
            }
        } while (!token.is(TokenType.end));
        return object;
    }

    private RomARRAY extractRomDataArray(
        final String name,
        final Token arrToken,
        final KeywordType arrayType
    ) throws ParseException {
        final int beg = arrToken.beg + 1;
        final int end = arrToken.end - 1;
        int i = beg;
        Token token;
        final RomARRAY array = RomARRAY.of(name, null);
        do {
            final RomDataWrapper wrap = getRomData(i, end, arrayType);
            final RomData data = wrap.getData();
            token = wrap.getToken();
            i = token.beg;
            if (!(data instanceof RomEMPTY)) {
                array.add(data);
            }
        } while (!token.is(TokenType.end));
        return array;
    }

    private char extractChar(final Token[] tokens, final int end) {
        final Token token = tokens[0];
        switch (token.type) {
            case string:
            case oneChar: {
                final String str = token.getString();
                return StringEscapeUtils.unescapeJava(str.substring(1, str.length() - 1)).charAt(0);
            }
            case number:
                break;
            default:
                break;
        }
        return 0;
    }

    private String extractString(final Token[] tokens, final int end) {
        final Token token = tokens[0];
        switch (token.type) {
            case string:
            case oneChar: {
                final String str = token.getString();
                return StringEscapeUtils.unescapeJava(str.substring(1, str.length() - 1));
            }
            case number:
                break;
            default:
                break;
        }
        return null;
    }

    private int extractInteger(final Token[] tokens, final int end) throws ParseException {
        Token token = tokens[0];

        int sign = 1;
        if (token.is(TokenType.delimit)) {
            switch (token.getString()) {
                case "+":
                    break;
                case "-":
                    sign = -1;
                    break;
                default:
                    throw new ParseException(token, "Непонятный знак");
            }
            token = token.next(end);
        }
        int out = 0;
        switch (token.type) {
            case number:
                out = Integer.parseInt(token.getString()) * sign;
                break;
            case real:
                out = Double.valueOf(token.getString()).intValue() * sign;
                break;
            case string:
            case oneChar: {
                final String str = token.getString();
                out = Integer.parseInt(str.substring(1, str.length() - 1)) * sign;
            }
            break;
            default:
                throw new ParseException(token, "Ошибка разбора");
        }
        tokens[0] = token;
        return out;
    }

    private double extractDouble(final Token[] tokens, final int end) throws ParseException {
        Token token = tokens[0];

        double sign = 1.0;
        if (token.is(TokenType.delimit)) {
            switch (token.getString()) {
                case "+":
                    break;
                case "-":
                    sign = -1.0;
                    break;
                default:
                    throw new ParseException(token, "Непонятный знак");
            }
            token = token.next(end);
        }
        double out;
        switch (token.type) {
            case number:
            case real:
                out = Double.parseDouble(token.getString()) * sign;
                break;
            case string:
            case oneChar: {
                final String str = token.getString();
                out = Double.parseDouble(str.substring(1, str.length() - 1)) * sign;
            }
            break;
            default:
                throw new ParseException(token, "Ошибка разбора");
        }
        tokens[0] = token;
        return out;
    }
}
