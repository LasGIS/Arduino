/*
 *  @(#)DataCppLoader.java  last: 30.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.load.compile.ParseException;
import com.lasgis.arduino.eeprom.load.compile.TokenParser;
import com.lasgis.arduino.eeprom.memory.RomCHAR;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.util.Util;
import com.sun.org.apache.xalan.internal.xsltc.compiler.CompilerException;

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
            token = addRomData(i, end);
            i = token.end + 1;
        } while (token.is(TokenType.delimit, ";"));
    }

    private Token addRomData(final int beg, final int end) throws ParseException {
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
        RomData data;
        if (type != null) {
            switch (type) {
                case CHAR: {
                    token = token.next(end);
                    token.assertion(TokenType.delimit, "'");
                    token = token.next(end);
                    char chr = token.getString().charAt(0);
                    token = token.next(end);
                    token.assertion(TokenType.delimit, "'");
                    data = RomCHAR.of(name, chr);
                } break;
            }
        } else {
            throw new ParseException(token, "Expected type of Rom Data.");
        }

        return token;
    }
}
