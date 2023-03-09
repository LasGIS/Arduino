/*
 *  @(#)DataCppLoader.java  last: 09.03.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.load.compile.ParseException;
import com.lasgis.arduino.eeprom.load.compile.RomDataWrapper;
import com.lasgis.arduino.eeprom.load.compile.TokenParser;
import com.lasgis.arduino.eeprom.memory.RomARRAY;
import com.lasgis.arduino.eeprom.memory.RomCHAR;
import com.lasgis.arduino.eeprom.memory.RomDOUBLE;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.arduino.eeprom.memory.RomEMPTY;
import com.lasgis.arduino.eeprom.memory.RomFLOAT;
import com.lasgis.arduino.eeprom.memory.RomINT16;
import com.lasgis.arduino.eeprom.memory.RomINT32;
import com.lasgis.arduino.eeprom.memory.RomINT8;
import com.lasgis.arduino.eeprom.memory.RomOBJECT;
import com.lasgis.arduino.eeprom.memory.RomSTRING;
import com.lasgis.util.Util;
import lombok.Getter;
import org.apache.commons.lang3.StringEscapeUtils;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

/**
 * Загрузчик из *.data файла.
 * Это с-подобная структура.
 *
 * @author Vladimir Laskin
 * @since 30.05.2018
 */
class DataCppLoader extends TokenParser {

    @Getter
    private List<RomData> list = new ArrayList<>();

    public DataCppLoader(final StringBuilder prg) {
        super(prg);
        list.clear();
    }

    static List<RomData> load(final File file) throws ParseException {
        final DataCppLoader loader = new DataCppLoader(Util.loadStringFromFile(file));
        loader.parse();
        return loader.list;
    }

    public void parse() throws ParseException {
        final int beg = 0;
        final int end = sb.length() - 1;
        int i = beg;
        Token token;
        do {
            final RomDataWrapper wrap = getRomData(i, end, null);
            final RomData data = wrap.getData();
            token = wrap.getToken();
            i = token.end + 1;
            if (token.is(TokenType.delimit, ";") && !(data instanceof RomEMPTY)) {
                list.add(data);
            }
        } while (!token.is(TokenType.end));
    }

    private RomDataWrapper getRomData(
        final int beg, final int end,
        final KeywordType expectedType
    ) throws ParseException {
        Token token = nextToken(beg, end).SkipComment(end);
        if (token.is(TokenType.end)) {
            return RomDataWrapper.of(RomEMPTY.of(), token);
        }
        KeywordType type = expectedType;
        KeywordType arrayType = null;
        String name = null;

        if (token.is(TokenType.keyword)) {
            type = KeywordType.of(token.getString());
            token = token.next(end).SkipComment(end);
        }
        if (token.is(TokenType.name)) {
            name = token.getString();
            token = token.next(end).SkipComment(end);
        }
        if (token.is(TokenType.keyword)) {
            arrayType = KeywordType.of(token.getString());
            token = token.next(end).SkipComment(end);
        }
        if (token.is(TokenType.delimit, ":")) {
            token = token.next(end);
        }
        RomData data = RomEMPTY.of();
        final Token[] tokens = {token};
        if (type != null) {
            switch (type) {
                case CHAR: {
                    final char chr = extractChar(tokens, end);
                    data = RomCHAR.of(name, null, chr);
                } break;
                case INT8: {
                    final int value = extractInteger(tokens, end);
                    data = RomINT8.of(name, null, value);
                } break;
                case INT16: {
                    final int value = extractInteger(tokens, end);
                    data = RomINT16.of(name, null, value);
                } break;
                case INT32: {
                    final int value = extractInteger(tokens, end);
                    data = RomINT32.of(name, null, value);
                } break;
                case FLOAT: {
                    final double value = extractDouble(tokens, end);
                    data = RomFLOAT.of(name, null, value);
                } break;
                case DOUBLE: {
                    final double value = extractDouble(tokens, end);
                    data = RomDOUBLE.of(name, null, value);
                } break;
                case STRING: {
                    final String value = extractString(tokens, end);
                    data = (value == null) ? RomEMPTY.of(name) : RomSTRING.of(name, null, value);
                } break;
                default:
                    break;
            }
            token = tokens[0];
        } else if (token.is(TokenType.block, "{")) {
            data = getRomDataObject(name, token);
        } else if (token.is(TokenType.block, "[")) {
            data = getRomDataArray(name, token, arrayType);
        } else {
            throw new ParseException(token, "Expected type of Rom Data.");
        }
        token = token.next(end).SkipComment(end);
        return RomDataWrapper.of(data, token);
    }

    private RomOBJECT getRomDataObject(final String name, final Token arrToken) throws ParseException {
        final int beg = arrToken.beg + 1;
        final int end = arrToken.end - 1;
        int i = beg;
        Token token;
        final RomOBJECT object = RomOBJECT.of(name, null);
        do {
            final RomDataWrapper wrap = getRomData(i, end, null);
            final RomData data = wrap.getData();
            token = wrap.getToken();
            i = token.end + 1;
            if ((token.is(TokenType.delimit, ",") || token.is(TokenType.end)) && !(data instanceof RomEMPTY)) {
                object.add(data);
            }
        } while (!token.is(TokenType.end));
        return object;
    }

    private RomARRAY getRomDataArray(
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
            i = token.end + 1;
            if ((token.is(TokenType.delimit, ",") || token.is(TokenType.end)) && !(data instanceof RomEMPTY)) {
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
            } break;
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
        double out = 0.0;
        switch (token.type) {
            case number:
            case real:
                out = Double.valueOf(token.getString()) * sign;
                break;
            case string: {
                final String str = token.getString();
                out = Double.valueOf(str.substring(1, str.length() - 1)) * sign;
            } break;
            case oneChar: {
                final String str = token.getString();
                out = Double.valueOf(str.substring(1, str.length() - 1)) * sign;
            } break;
            default:
                throw new ParseException(token, "Ошибка разбора");
        }
        tokens[0] = token;
        return out;
    }
}
