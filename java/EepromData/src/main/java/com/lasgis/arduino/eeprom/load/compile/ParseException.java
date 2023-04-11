/*
 *  @(#)ParseException.java  last: 11.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load.compile;

/**
 * Ошибка при разборе.
 * @author Vladimir Laskin
 * @version 1.0
 */
public class ParseException extends Exception {

    /**
     * Описание одной строки.
     */
    private class Line {
        int count;
        int beg;
        int end;
        String str;
    }
    private int row = 1;
    private int col = 1;
    private final Line[] lines = new Line[3];

    /**
     * @param token token
     * @param message описание ошибки
     */
    public ParseException(final TokenParser.Token token, final String message) {
        super(message);
        final StringBuilder sb = token.getSb();
        Line curLine = new Line();
        curLine.beg = 0;
        char ch0 = ' ';
        int  i = 0;
        for (; i < token.beg; i++) {
            final char ch = sb.charAt(i);
            if (ch == '\r' || ch == '\n') {
                if (!(ch0 == '\r' && ch == '\n')) {
                    curLine.end = i;
                    curLine.count = row++;
                    lines[0] = lines[1];
                    lines[1] = curLine;
                    curLine = new Line();
                    curLine.beg = i + 1;
                }
                col = 1;
            } else {
                curLine.end = i;
                col++;
            }
            ch0 = ch;
        }
        lines[0] = lines[1];
        for (int j = 1; i < sb.length() && j < 3; i++) {
            final char ch = sb.charAt(i);
            if (ch == '\r' || ch == '\n') {
                if (!(ch0 == '\r' && ch == '\n')) {
                    curLine.end = i;
                    curLine.count = row + j - 1;
                    lines[j] = curLine;
                    curLine = new Line();
                    curLine.beg = i + 1;
                    j++;
                }
            } else {
                curLine.end = i;
            }
            ch0 = ch;
        }
        for (Line line : lines) {
            if (line != null) {
                line.str = sb.substring(line.beg, line.end);
            }
        }
    }

    /**
     * .
<pre>
19  :     routine smartRunTo(endCell) {
20  :         until(endCell != nextPoint) {
                            ^
21  :             FindWay(endCell) nextPoint;
</pre>
     * @return строки с информацией об ошибки
     */
    @Override
    public String getMessage() {
        final StringBuilder sb = new StringBuilder();
        sb.append(String.format("Parser error in(%1$d,%2$d) - ", row, col));
        sb.append(super.getMessage());
        sb.append(":\n\n");
        for (int i = 0; i < 3; i++) {
            if (lines[i] != null) {
                sb.append(String.format("%1$4d: %2$s\n", lines[i].count, lines[i].str));
            }
            if (i == 1) {
                for (int j = 0; j < col + 5; j++) {
                    sb.append(' ');
                }
                sb.append("^\n");
            }
        }
        //sb.append("\n");
        return sb.toString();
    }
}
