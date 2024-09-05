/*
 *  @(#)HeadLoader.java  last: 06.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.load;

import com.lasgis.arduino.editfont.data.FontData;
import com.lasgis.arduino.editfont.load.compile.ParseException;
import com.lasgis.arduino.editfont.load.compile.TokenParser;
import com.lasgis.arduino.editfont.load.compile.TokenWrapper;
import com.lasgis.arduino.editfont.load.data.CppOperator;
import com.lasgis.arduino.editfont.load.data.TypeOperator;
import com.lasgis.util.Util;

import java.io.File;

import static java.util.Objects.nonNull;

/**
 * Загрузчик из *.data файла. Это с-подобная структура.
 *
 * @author Vladimir Laskin
 * @since 30.05.2018
 */
class HeadLoader extends TokenParser {

    public HeadLoader() {
        super();
    }

    static void load(final FontData fontData, final File file) throws ParseException {
        final HeadLoader loader = new HeadLoader();
        loader.loadFile(fontData, file);
    }

    private void loadFile(final FontData fontData, final File file) throws ParseException {
        fontData.setHFileName(file.getName());
        fontData.setHSource(Util.loadStringFromFile(file));
        setProgramCode(fontData.getHSource());
        parse(fontData);
    }

    public void parse(final FontData fontData) throws ParseException {
        final int beg = 0;
        final int end = sb.length() - 1;
        int i = beg;
        Token token;
        do {
            final TokenWrapper<CppOperator> operatorWrap = getOperator(i, end);
            if (nonNull(operatorWrap)) {
                final CppOperator cppOperator = operatorWrap.getData();
                token = operatorWrap.getToken();
            } else {
                token = nextToken(i, end).SkipComment(end);
            }
            i = token.beg;
        } while (!token.is(TokenType.end));
    }

    private TokenWrapper<CppOperator> getOperator(
        final int beg, final int end
    ) throws ParseException {
        Token token = nextToken(beg, end).SkipComment(end);
        final CppOperator cppOperator = CppOperator.of();
        if (token.is(TokenType.delimit, "#")) {
            token = token.next(end).SkipComment(end);
            if (token.is(TokenType.keyword, KeywordType.INCLUDE.getName())) {
                cppOperator.setType(TypeOperator.include);
                token = token.next(end).SkipComment(end).assertion(TokenType.block);
                cppOperator.setValue(token.getString());
                token = token.next(end).SkipComment(end);
                return TokenWrapper.of(cppOperator, token);
            } else if (token.is(TokenType.keyword, KeywordType.DEFINE.getName())) {
                cppOperator.setType(TypeOperator.define);
                token = token.next(end).SkipComment(end).assertion(TokenType.name);
                cppOperator.setName(token.getString());
                token = token.next(end).SkipComment(end);
                if (token.type == TokenType.number) {
                    cppOperator.setValue(token.getString());
                    token = token.next(end).SkipComment(end);
                    return TokenWrapper.of(cppOperator, token);
                }
                throw new ParseException(token, "Ошибка разбора");
            }

        } else {
            while (token.is(TokenType.keyword,
                KeywordType.PROGMEM.getName(),
                KeywordType.CONST.getName(),
                KeywordType.UNSIGNED.getName(),
                KeywordType.CHAR.getName(),
                KeywordType.EXTERN.getName()
            )) {
                token = token.next(end).SkipComment(end);
            }
            cppOperator.setType(TypeOperator.field);
            token.assertion(TokenType.name);
            cppOperator.setName(token.getString());
            token = token.next(end).SkipComment(end);
            switch (token.type) {
                case block:
                case number:
                    cppOperator.setValue(token.getString());
            }
            token = token.next(end).SkipComment(end).assertion(TokenType.delimit, ";");
            token = token.next(end).SkipComment(end);
            return TokenWrapper.of(cppOperator, token);
        }
        return null;
    }
}
