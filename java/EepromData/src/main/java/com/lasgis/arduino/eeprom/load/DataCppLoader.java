/*
 *  @(#)DataCppLoader.java  last: 14.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.load.compile.ParseException;
import com.lasgis.arduino.eeprom.load.compile.TokenParser;
import com.lasgis.arduino.eeprom.load.compile.TokenWrapper;
import com.lasgis.arduino.eeprom.memory.BatchMemory;
import com.lasgis.arduino.eeprom.memory.MemoryRoms;
import com.lasgis.arduino.eeprom.memory.RomADDRESS;
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
import lombok.Data;
import lombok.NoArgsConstructor;
import org.apache.commons.lang3.StringEscapeUtils;

import javax.xml.bind.DatatypeConverter;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

import static com.lasgis.arduino.eeprom.load.compile.TokenParser.KeywordType.BATCH_MEMORY;
import static com.lasgis.arduino.eeprom.load.compile.TokenParser.KeywordType.MEMORY_ROMS;
import static com.lasgis.util.Util.parseHexByte;
import static com.lasgis.util.Util.parseHexInt;
import static com.lasgis.util.Util.parseHexShort;
import static java.util.Objects.isNull;
import static java.util.Objects.nonNull;

/**
 * Загрузчик из *.data файла.
 * Это с-подобная структура.
 *
 * @author Vladimir Laskin
 * @since 30.05.2018
 */
class DataCppLoader extends TokenParser {

    @NoArgsConstructor(staticName = "of")
    @Data
    static class RomParameters {
        private String refId;
    }

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
            final TokenWrapper<RomData> wrap = getRomData(i, end, null);
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
                    batch.setDevice(extractByte(tokens, end));
                    break;
                case "prefix":
                    batch.setPrefix(extractString(tokens, end));
                    break;
                case "address":
                    batch.setAddress(extractInt(tokens, end));
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

    private TokenWrapper<RomData> getRomData(
        final int beg, final int end,
        final KeywordType expectedType
    ) throws ParseException {
        Token token = nextToken(beg, end).SkipComment(end);
        if (token.is(TokenType.end)) {
            return TokenWrapper.of(RomEMPTY.of(), token);
        }
        /* тип основного RomData */
        KeywordType type = expectedType;
        /* дополнительный тип для RomARRAY */
        KeywordType arrayType = null;
        /* наименование основного RomData */
        String name = null;
        String refId = null;

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
        /* блок параметров с пока единственным 'refId' */
        if (token.is(TokenType.block, "(")) {
            final RomParameters parameters = extractParameters(token);
            refId = parameters.getRefId();
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
                    data = RomCHAR.of(name, refId, chr);
                }
                break;
                case INT8: {
                    final byte value = extractByte(tokens, end);
                    data = RomINT8.of(name, refId, value);
                }
                break;
                case INT16: {
                    final short value = extractShort(tokens, end);
                    data = RomINT16.of(name, refId, value);
                }
                break;
                case INT32: {
                    final int value = extractInt(tokens, end);
                    data = RomINT32.of(name, refId, value);
                }
                break;
                case FLOAT: {
                    final double value = extractDouble(tokens, end);
                    data = RomFLOAT.of(name, refId, value);
                }
                break;
                case STRING: {
                    final String value = extractString(tokens, end);
                    data = (value == null) ? RomEMPTY.of(name) : RomSTRING.of(name, refId, value);
                }
                break;
                case OBJECT: {
                    token.assertion(TokenType.block, "{");
                    data = extractRomDataObject(name, refId, token);
                }
                break;
                case ARRAY: {
                    token.assertion(TokenType.block, "[");
                    data = extractRomDataArray(name, refId, token, arrayType);
                }
                break;
                case EEPROM_ADDRESS: {
                    if (isNull(refId)) {
                        throw new ParseException(token, "Need to fill 'refId' params");
                    }
                    data = RomADDRESS.of(name, refId);
                }
                break;
                case DUMP: {
                    data = extractDump(name, refId, token);
                }
                break;
                default:
                    throw new ParseException(token, "Need to parse type: \"" + type + "\"");
            }
            token = tokens[0];
        } else if (token.is(TokenType.block, "{")) {
            data = extractRomDataObject(name, refId, token);
        } else if (token.is(TokenType.block, "[")) {
            data = extractRomDataArray(name, refId, token, arrayType);
        } else {
            throw new ParseException(token, "Expected type of Rom Data.");
        }
        token = token.next(end);
        token = token.SkipComment(end);
        token = token.assertion(TokenType.delimit, ";", ",");
        token = token.next(end);
        token = token.SkipComment(end);
        return TokenWrapper.of(data, token);
    }

    private RomParameters extractParameters(final Token blockToken) throws ParseException {
        final int beg = blockToken.beg + 1;
        final int end = blockToken.end - 1;
        int i = beg;
        Token token;
        final RomParameters parameters = RomParameters.of();
        do {
            token = nextToken(i, end).SkipComment(end).assertion(TokenType.name, "refId");
            final String key = token.getString();
            token = token.next(end).SkipComment(end).assertion(TokenType.delimit, ":");
            token = token.next(end).SkipComment(end);
            final Token[] tokens = {token};
            if ("refId".equals(key)) {
                parameters.setRefId(extractString(tokens, end));
            } else {
                throw new ParseException(token, "Непонятный параметр \"" + key + "\"");
            }
            token = tokens[0].next(end)
                .SkipComment(end)
                .Skip(TokenType.delimit, end)
                .SkipComment(end);
            i = token.beg;
        } while (!token.is(TokenType.end));
        return parameters;
    }

    private RomOBJECT extractRomDataObject(
        final String name, final String refId, final Token objToken
    ) throws ParseException {
        final int beg = objToken.beg + 1;
        final int end = objToken.end - 1;
        int i = beg;
        Token token;
        final RomOBJECT object = RomOBJECT.of(name, refId);
        do {
            final TokenWrapper<RomData> wrap = getRomData(i, end, null);
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
        final String name, final String refId, final Token arrToken, final KeywordType arrayType
    ) throws ParseException {
        final int beg = arrToken.beg + 1;
        final int end = arrToken.end - 1;
        int i = beg;
        Token token;
        final RomARRAY array = RomARRAY.of(name, refId);
        do {
            final TokenWrapper<RomData> wrap = getRomData(i, end, arrayType);
            final RomData data = wrap.getData();
            token = wrap.getToken();
            i = token.beg;
            if (!(data instanceof RomEMPTY)) {
                array.add(data);
            }
        } while (!token.is(TokenType.end));
        return array;
    }

    private RomARRAY extractDump(final String name, final String refId, final Token arrToken) throws ParseException {
        arrToken.assertion(TokenType.block, "[");
        final RomARRAY rom = RomARRAY.of(name, refId);
        final String dumpStr = arrToken.getString().trim().replaceAll("[ \\u005d\\u005b\n\t\r]", "");
        byte[] dump = DatatypeConverter.parseHexBinary(dumpStr);
        for (final byte bt : dump) {
            rom.add(RomINT8.of(bt));
        }
        return rom;
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

    private String extractString(final Token[] tokens, final int end) throws ParseException {
        final Token token = tokens[0];
        token.assertion(TokenType.string);
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

    /**
     * Собираем целое
     *
     * @param tokens обертка над token
     * @return собранная строка
     */
    private String intStringCollection(final Token[] tokens, final int end) {
        Token token = tokens[0];
        String[] validHex = {"a", "b", "c", "d", "e", "f", "A", "B", "C", "D", "E", "F",};
        final StringBuilder sb = new StringBuilder();
        if (token.is(TokenType.delimit, "+", "-")) {
            sb.append(token.getString());
            token = token.next(end);
        }
        if (token.is(TokenType.number, "0x")) {
            token.end = token.beg + 1;
            while (token.is(TokenType.number) || token.is(TokenType.name, validHex)) {
                sb.append(token.getString());
                tokens[0] = token;
                token = token.next(end);
            }
        } else if (token.is(TokenType.number)) {
            sb.append(token.getString());
            tokens[0] = token;
        }
        return sb.toString();
    }


    private byte extractByte(final Token[] tokens, final int end) throws ParseException {
        final String intString = intStringCollection(tokens, end);
        return parseHexByte(intString);
    }

    private short extractShort(final Token[] tokens, final int end) throws ParseException {
        final String intString = intStringCollection(tokens, end);
        return parseHexShort(intString);
    }

    private int extractInt(final Token[] tokens, final int end) throws ParseException {
        final String intString = intStringCollection(tokens, end);
        return parseHexInt(intString);
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
