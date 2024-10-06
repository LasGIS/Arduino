/*
 *  @(#)TokenParser.java  last: 06.10.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

/**
 * Здесь осуществляется Предварительный разбор языка на отдельные элементы.
 *
 * @author Vladimir Laskin
 * @version 1.0
 */
public class TokenParser {

    private static final String SPACE_CHARS = " \n\r\t";
    private static final String DELIMIT_CHARS = ".,;:=<>(){}[]+-*/!|\"'\\№&%?@#$%^";
    private static final String START_BLOCK_CHARS = "({[<";
    private static final List<String> OPERATORS = Arrays.asList(
        "=", "+=", "-=", "*=", "/=", "+", "-", "*", "/", "&&", "||", ">=", "<", "<=", ">", ">=", "==", "!=", "!", "."
    );
    private static final String UNARY_DELIMIT = ",;:";
    private static final Map<Character, Character> BLOCK_CHARS = Map.of('(', ')', '{', '}', '[', ']', '<', '>');
    private static final String DIGIT_CHARS = "0123456789";

    protected final StringBuilder sb;
    protected final KeywordType keywordType;

    /**
     * Типовые знаки.
     */
    public enum CharType {
        /** Буква, цифра, разделители, пробельные символы. */
        letter, digit, delimitChars, space
    }

    /**
     * Простой конструктор.
     */
    public TokenParser(KeywordType keywordType) {
        this.keywordType = keywordType;
        sb = new StringBuilder();
    }

    /**
     * Конструктор с заполнением кода.
     *
     * @param prg код программы
     */
    public TokenParser(final KeywordType keywordType, final StringBuilder prg) {
        this.keywordType = keywordType;
        this.sb = prg;
    }

    /**
     * Получаем очередной Token (отдельное слово, число или разделитель).
     *
     * @param beg начало разбора
     * @param end конец разбора
     * @return порция с типом
     */
    public Token nextToken(final int beg, final int end) {
        int i = beg;
        final Token token = new Token(this);
        Character start = null;
        loop:
        while (i <= end) {
            final char ch = sb.charAt(i);
            final CharType cl = charLevel(ch);
            switch (cl) {
                default:
                case space:
                    if (start != null) {
                        break loop;
                    }
                    break;
                case letter:
                    if (start == null) {
                        token.beg = i;
                        token.type = TokenType.name;
                        start = ch;
                    } else if (token.type == TokenType.delimit
                        || token.type == TokenType.number
                        || token.type == TokenType.real) {
                        break loop;
                    }
                    break;
                case digit:
                    if (start == null) {
                        token.beg = i;
                        token.type = TokenType.number;
                        start = ch;
                    } else if (token.type == TokenType.delimit) {
                        break loop;
                    }
                    break;
                case delimitChars:
                    if (start == null) {
                        token.beg = i;
                        if (START_BLOCK_CHARS.indexOf(ch) >= 0) {
                            token.end = end;
                            return getBlock(token);
                        } else if (ch == '\"') {
                            token.end = end;
                            return getString(token);
                        } else if (ch == '\'') {
                            token.end = end;
                            return getChar(token);
                        }
                        token.type = TokenType.delimit;
                        start = ch;
                    } else {
                        if (ch == '.' && token.type == TokenType.number) {
                            token.type = TokenType.real;
                        } else if (token.type != TokenType.delimit) {
                            break loop;
                        } else if (UNARY_DELIMIT.indexOf(start) >= 0) {
                            break loop;
//                        } else if (("!+-(".indexOf((int) ch) >= 0) && OPERATORS.contains(sb.substring(token.beg, i))) {
//                            break loop;
                        }
                    }
                    break;
            }
            i++;
        }
        if (i > end && token.type == TokenType.start) {
            token.type = TokenType.end;
            token.beg = end;
            token.end = end;
            return token;
        }
        token.end = i - 1;
        // дополнительные проверки
        if (token.type != null) {
            switch (token.type) {
                case error:
                    break;
                case keyword:
                    break;
                case name: {
                    final String str = token.getString();
                    if (keywordType.of(str) != null) {
                        token.type = TokenType.keyword;
                    }
                    break;
                }
                case delimit: {
                    final String str = token.getString();
                    /*if (OPERATORS.contains(str)) {
                        token.type = TokenType.operator;
                    } else */
                    if (str.length() >= 2 && "/*".equals(str.substring(0, 2))) {
                        token.end = end;
                        return getInnerComment(token);
                    } else if (str.equals("//")) {
                        token.end = end;
                        return getOuterComment(token);
                    }
                    break;
                }
                default:
                    break;
            }
        }
        return token;
    }

    /**
     * Один раз код программы надо добавить.
     *
     * @param programCode код программы
     */
    public void setProgramCode(final StringBuilder programCode) {
        sb.append(programCode);
    }

    /**
     * Получаем очередной Token (отдельное слово, число или разделитель).
     *
     * @param token token для получения начало разбора
     * @param end   конец разбора
     * @return от порции с типом
     */
    public Token nextToken(final Token token, final int end) {
        return nextToken(token.end + 1, end);
    }

    /**
     * Типы токенов.
     */
    public enum TokenType {
        /** Ошибка; ключевое слово; имя переменной или метода, ... */
        error, keyword, name, delimit, string, oneChar, number, real, block, operator, comment, start, end
    }

    private Token getInnerComment(final Token token) {
        final int end = sb.indexOf("*/", token.beg);
        if (end < 0 || end > token.end) {
            token.type = TokenType.error;
        } else {
            token.type = TokenType.comment;
            token.end = end + 1;
        }
        return token;
    }

    private Token getOuterComment(final Token token) {
        int end = sb.indexOf("\r", token.beg);
        final int endN = sb.indexOf("\n", token.beg);
        if (end < 0 || (endN >= 0 && endN < end)) {
            end = endN;
        }
        if (end < 0 || end > token.end) {
            token.type = TokenType.error;
        } else {
            token.type = TokenType.comment;
            token.end = end;
        }
        return token;
    }

    private Token getBlock(final Token token) {
        token.type = TokenType.block;
        int i = token.beg;
        int count = 0;
        final char firstChar = sb.charAt(i++);
        final char endChar = BLOCK_CHARS.get(firstChar);
        while (i <= token.end) {
            final char ch = sb.charAt(i);
            final CharType cl = charLevel(ch);
            switch (cl) {
                case delimitChars:
                    if (ch == firstChar) {
                        count++;
                    } else if (ch == endChar) {
                        if (count == 0) {
                            token.end = i;
                            return token;
                        } else {
                            count--;
                        }
                    }
                    break;
                default:
                    break;
            }
            i++;
        }
        token.end = i - 1;
        token.type = TokenType.error;
        return token;
    }

    private Token getString(final Token token) {
        token.type = TokenType.string;
        int i = token.beg + 1;
        char ch1 = ' ';
        while (i <= token.end) {
            final char ch = sb.charAt(i);
            final CharType cl = charLevel(ch);
            switch (cl) {
                case delimitChars:
                    if (ch == '\"' && ch1 != '\\') {
                        token.end = i;
                        return token;
                    }
                    break;
                default:
                    break;
            }
            ch1 = ch;
            i++;
        }
        token.end = i - 1;
        token.type = TokenType.error;
        return token;
    }

    private Token getChar(final Token token) {
        token.type = TokenType.oneChar;
        int i = token.beg + 1;
        char ch1 = ' ';
        while (i <= token.end) {
            final char ch = sb.charAt(i);
            final CharType cl = charLevel(ch);
            switch (cl) {
                case delimitChars:
                    if (ch == '\'' && ch1 != '\\') {
                        token.end = i;
                        return token;
                    }
                    break;
                default:
                    break;
            }
            ch1 = ch;
            i++;
        }
        token.end = i - 1;
        token.type = TokenType.error;
        return token;
    }

    private CharType charLevel(final char ch) {
        if (DIGIT_CHARS.indexOf(ch) >= 0) {
            return CharType.digit;
        } else if (DELIMIT_CHARS.indexOf(ch) >= 0) {
            return CharType.delimitChars;
        } else if (SPACE_CHARS.indexOf(ch) >= 0) {
            return CharType.space;
        } else {
            return CharType.letter;
        }
    }
}
