/*
 * @(#)DataCppLoader.java
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright © 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.load.compile.ParseException;
import com.lasgis.arduino.eeprom.load.compile.RomDataWrapper;
import com.lasgis.arduino.eeprom.load.compile.TokenParser;
import com.lasgis.arduino.eeprom.memory.RomCHAR;
import com.lasgis.arduino.eeprom.memory.RomDOUBLE;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.arduino.eeprom.memory.RomEMPTY;
import com.lasgis.arduino.eeprom.memory.RomFLOAT;
import com.lasgis.arduino.eeprom.memory.RomINT16;
import com.lasgis.arduino.eeprom.memory.RomINT32;
import com.lasgis.arduino.eeprom.memory.RomINT8;
import com.lasgis.util.Util;
import org.apache.commons.lang.StringEscapeUtils;

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

    private final static String UNKNOWN_FORMAT = "Формат XML файла: ";

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
            final RomDataWrapper wrap = getRomData(i, end);
            final RomData data = wrap.getData();
            token = wrap.getToken();
            i = token.end + 1;
            if (token.is(TokenType.delimit, ";") && !(data instanceof RomEMPTY)) {
                list.add(data);
            }
        } while (!token.is(TokenType.end));
    }

    private RomDataWrapper getRomData(final int beg, final int end) throws ParseException {
        Token token = nextToken(beg, end).SkipComment(end);
        KeywordType type = null;
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
        token.assertion(TokenType.delimit, ":");
        RomData data = RomEMPTY.of();
        final Token[] tokens = {token};
        if (type != null) {
            switch (type) {
                case CHAR: {
                    token = token.next(end);
                    final char chr = extractChar(token);
                    data = RomCHAR.of(name, chr);
                } break;
                case INT8: {
                    final int value = extractInteger(tokens, end);
                    token = tokens[0];
                    data = RomINT8.of(name, value);
                } break;
                case INT16: {
                    final int value = extractInteger(tokens, end);
                    token = tokens[0];
                    data = RomINT16.of(name, value);
                } break;
                case INT32: {
                    final int value = extractInteger(tokens, end);
                    token = tokens[0];
                    data = RomINT32.of(name, value);
                } break;
                case FLOAT: {
                    token = token.next(end);
                    final double value = extractDouble(token);
                    data = RomFLOAT.of(name, value);
                } break;
                case DOUBLE: {
                    token = token.next(end);
                    final double value = extractDouble(token);
                    data = RomDOUBLE.of(name, value);
                } break;
                default:
                    token = token.next(end);
                    break;
            }
        } else {
            throw new ParseException(token, "Expected type of Rom Data.");
        }
        token = token.next(end).SkipComment(end);
        return RomDataWrapper.of(data, token);
    }

    private char extractChar(final Token token) {
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

    private int extractInteger(final Token[] tokens, final int end) throws ParseException {
        Token token = tokens[0];
        token = token.next(end);

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
                out = Integer.valueOf(token.getString()) * sign;
                break;
            case real:
                out = Double.valueOf(token.getString()).intValue() * sign;
            break;
            case string: {
                final String str = token.getString();
                out = Integer.valueOf(str.substring(1, str.length() - 1)) * sign;
            } break;
            case oneChar: {
                final String str = token.getString();
                out = Integer.valueOf(str.substring(1, str.length() - 1)) * sign;
            } break;
            default:
                throw new ParseException(token, "Ошибка разбора");
        }
        tokens[0] = token;
        return out;
    }

    private double extractDouble(final Token token) {
        switch (token.type) {
            case number:
            case real:
                return Double.valueOf(token.getString()).intValue();
            case string: {
                final String str = token.getString();
                return Double.valueOf(str.substring(1, str.length() - 1));
            }
            case oneChar: {
                final String str = token.getString();
                return Double.valueOf(str.substring(1, str.length() - 1));
            }
            default:
                break;
        }
        return 0;
    }
}
