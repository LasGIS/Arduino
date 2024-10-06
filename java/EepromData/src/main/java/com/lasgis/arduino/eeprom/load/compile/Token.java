/*
 *  @(#)Token.java  last: 06.10.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load.compile;

/**
 * Лексема разбираемого кода.
 *
 * @author Vladimir Laskin
 * @since 06.10.2024 : 13:23
 */
public class Token {

    private final TokenParser tokenParser;
    public TokenParser.TokenType type;
    public int beg;
    public int end;

    /**
     * Пустой конструктор.
     */
    public Token(TokenParser tokenParser) {
        this(tokenParser, 0, 0);
    }

    /**
     * Передаем только область.
     *
     * @param beg начало области
     * @param end конец области
     */
    public Token(TokenParser tokenParser, final int beg, final int end) {
        this(tokenParser, TokenParser.TokenType.start, beg, end);
    }

    /**
     * Полный конструктор.
     *
     * @param type тип области
     * @param beg  начало области
     * @param end  конец области
     */
    public Token(TokenParser tokenParser, final TokenParser.TokenType type, final int beg, final int end) {
        this.tokenParser = tokenParser;
        this.type = type;
        this.beg = beg;
        this.end = end;
    }

    public String getString() {
        return tokenParser.sb.substring(beg, end + 1);
    }

    public StringBuilder getSb() {
        return tokenParser.sb;
    }

    @Override
    public String toString() {
        return String.format("%1$d-%2$d(%3$s) \"%4$s\"", beg, end, type.name(), getString());
    }

    /**
     * Получаем Token, следующий за этим.
     *
     * @param parsEnd конец разбора
     * @return от порции с типом
     */
    public Token next(final int parsEnd) {
        if (end >= parsEnd) {
            final Token token = new Token(this.tokenParser, parsEnd, parsEnd);
            token.type = TokenParser.TokenType.end;
            return token;
        }
        return tokenParser.nextToken(end + 1, parsEnd);
    }

    /**
     * Получаем первый Token внутри за этого.
     *
     * @param parsEnd конец разбора
     * @return от порции с типом
     */
    public Token first(final int parsEnd) {
        return tokenParser.nextToken(beg + 1, parsEnd);
    }

    /**
     * Проверка на тип лексемы.
     *
     * @param chkType тип лексемы
     * @return true если тип лексемы совпадает
     */
    public boolean is(final TokenParser.TokenType chkType) {
        return this.type == chkType;
    }

    /**
     * Проверка на тип лексемы.
     *
     * @param chkType тип лексемы
     * @param values  удачное начало лексемы
     * @return true если тип лексемы совпадает
     */
    public boolean is(final TokenParser.TokenType chkType, final String... values) {
        if (this.type == chkType) {
            for (final String val : values) {
                if (tokenParser.sb.indexOf(val, beg) == beg) {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * Пропускаем комментарий.
     *
     * @param chkEnd конец
     * @return лексема без комментария
     */
    public Token SkipComment(final int chkEnd) {
        return Skip(TokenParser.TokenType.comment, chkEnd);
    }

    /**
     * Пропускаем такую лексему.
     *
     * @param aType  тип Пропускаемой лексемы
     * @param chkEnd конец
     * @return следующая лексема
     */
    public Token Skip(final TokenParser.TokenType aType, final int chkEnd) {
        Token tokenOut = this;
        while (tokenOut.type == aType) {
            tokenOut = tokenOut.next(chkEnd);
        }
        return tokenOut;
    }

    /**
     * Проверка на соответствие токена конкретному типу
     *
     * @param tokenType тип токена для проверки
     * @return токен если все нормально
     * @throws ParseException ошибка если тип токена не соответствует
     */
    public Token assertion(final TokenParser.TokenType tokenType) throws ParseException {
        if (this.type != tokenType) {
            throw new ParseException(this, "Expected TokenType == " + tokenType.name());
        }
        return this;
    }

    /**
     * Проверка на соответствие токена конкретному типу
     *
     * @param tokenType тип токена для проверки
     * @param values    дополнительные параметры для проверки
     * @return токен если все нормально
     * @throws ParseException ошибка если тип токена не соответствует
     */
    public Token assertion(final TokenParser.TokenType tokenType, final String... values) throws ParseException {
        if (this.type == tokenType) {
            for (String val : values) {
                if (tokenParser.sb.indexOf(val, beg) == beg) {
                    return this;
                }
            }
        } else if (this.type == TokenParser.TokenType.end) {
            return this;
        }
        final StringBuilder sbx = new StringBuilder("Expected TokenType == \"");
        sbx.append(tokenType.name()).append("\"; with value == [\"");
        for (String val : values) {
            sbx.append(val).append("\", \"");
        }
        final int len = sbx.length();
        sbx.delete(len - 3, len);
        sbx.append("]");
        throw new ParseException(this, sbx.toString());
    }
}
