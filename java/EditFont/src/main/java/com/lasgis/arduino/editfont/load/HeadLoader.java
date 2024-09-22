/*
 *  @(#)HeadLoader.java  last: 22.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.load;

import com.lasgis.arduino.editfont.data.FontData;
import com.lasgis.arduino.editfont.data.FontDataAdapter;
import com.lasgis.arduino.editfont.data.FontDataPerformed;
import com.lasgis.arduino.editfont.load.compile.ParseException;
import com.lasgis.arduino.editfont.load.compile.TokenParser;
import com.lasgis.arduino.editfont.load.compile.TokenWrapper;
import com.lasgis.arduino.editfont.load.data.CppOperator;
import com.lasgis.arduino.editfont.load.data.TypeOperator;
import com.lasgis.util.Util;
import lombok.extern.slf4j.Slf4j;

import java.io.File;
import java.util.Objects;

import static com.lasgis.arduino.editfont.load.LoadHelper.BASE_LINE_KEY;
import static com.lasgis.arduino.editfont.load.LoadHelper.CHAR_HEIGHT_KEY;
import static com.lasgis.arduino.editfont.load.LoadHelper.DATA_SIZE_KEY;
import static com.lasgis.arduino.editfont.load.LoadHelper.FIRST_CHAR_KEY;
import static com.lasgis.arduino.editfont.load.LoadHelper.NUMBER_CHARS_KEY;
import static java.util.Objects.nonNull;

/**
 * Загрузчик из *.data файла. Это с-подобная структура.
 *
 * @author Vladimir Laskin
 * @since 30.05.2018
 */
@Slf4j
public class HeadLoader extends TokenParser {
    static final HeadLoader HEAD_LOADER = new HeadLoader();

    public HeadLoader() {
        super();
        FontDataPerformed.addListener(new FontDataAdapter() {
            @Override
            public void onChangeHFile(final File file) {
                HEAD_LOADER.loadFile(file);
            }

            @Override
            public void onChangeHSource(final StringBuilder stringBuilder) {
                try {
                    HEAD_LOADER.parse(stringBuilder);
                } catch (final ParseException ex) {
                    log.error(ex.getMessage(), ex);
                }
            }
        });
    }

    private void loadFile(final File file) {
        final StringBuilder sb = Util.loadStringFromFile(file);
        FontDataPerformed.setHSource(sb);
    }

    public void parse(final StringBuilder stringBuilder) throws ParseException {
        setProgramCode(stringBuilder);

        final FontData fontData = FontDataPerformed.getFontData();
        fontData.setWidthChar(8);

        final String fontKey = fontData.getFontKey();
        final int beg = 0;
        final int end = sb.length() - 1;
        int i = beg;
        Token token;
        do {
            final TokenWrapper<CppOperator> operatorWrap = getOperator(i, end);
            if (nonNull(operatorWrap)) {
                final CppOperator cppOperator = operatorWrap.getData();
                final String name = cppOperator.getName();
                final String value = cppOperator.getValue();
                if ((NUMBER_CHARS_KEY + fontKey).equals(name)) {
                    fontData.setNumberChars(Integer.valueOf(value));
                } else if ((CHAR_HEIGHT_KEY + fontKey).equals(name)) {
                    fontData.setCharHeight(Integer.valueOf(value));
                } else if ((BASE_LINE_KEY + fontKey).equals(name)) {
                    fontData.setBaseLine(Integer.valueOf(value));
                } else if ((DATA_SIZE_KEY + fontKey).equals(name)) {
                    fontData.setDataSize(Integer.valueOf(value));
                } else if ((FIRST_CHAR_KEY + fontKey).equals(name)) {
                    fontData.setFirstChar(Integer.valueOf(value));
                }
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

        } else if (token.is(TokenType.keyword, KeywordType.EXTERN.getName())) {
            token = skipPreDefine(token, end, cppOperator);
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

    private Token skipPreDefine(final Token inToken, final int end, final CppOperator cppOperator) {
        Token token = inToken.next(end);
        cppOperator.setType(TypeOperator.field);
        do {
            if (token.is(TokenType.keyword)) {
                final String keyword = token.getString();
                switch (Objects.requireNonNull(KeywordType.of(keyword))) {
                    case PROGMEM: {
                        log.trace("PROGMEM");
                    }
                    break;
                    case CONST: {
                        log.trace("CONST");
                    }
                    break;
                    case UNSIGNED: {
                        log.trace("UNSIGNED");
                    }
                    break;
                    case CHAR: {
                        log.trace("CHAR");
                    }
                    break;
                }
            }
//            token.Skip(TokenType.operator, "*")) {
//                token = token.next(end).SkipComment(end);
//            }
            token = token.next(end).SkipComment(end);
        } while (!token.is(TokenType.name));
        return token;
    }
}
