/*
 * @(#)LGFormatter.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2013 LasGIS Company. All Rights Reserved.
 */
package com.lasgis.util;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.text.NumberFormat;
import java.text.SimpleDateFormat;
import java.util.Locale;
import java.util.TimeZone;

/**
 * The Class LGFormatter. Преобразование десятичного числа или даты в строку.
 *
 * @author Laskin
 * @version 1.0
 * @since 02.01.13 23:48
 */
public final class LGFormatter {

    /**
     * Полное число с секундами.
     */
    public static final SimpleDateFormat DATE_FORMAT = new SimpleDateFormat("DD HH:mm:ss");
    static {
        DATE_FORMAT.setTimeZone(TimeZone.getTimeZone("GMT"));
    }
    /**
     * Формат даты, используемый для логгирования.
     */
    public static final SimpleDateFormat LOG_DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss,SS");
    /**
     * Вывод десятичного числа.
     */
    public static final NumberFormat DECIMAL_FORMAT = new DecimalFormat(
        "###0.000", new DecimalFormatSymbols(Locale.ENGLISH)
    );

    private LGFormatter() {
    }

    /**
     * Форматированный вывод десятичного числа.
     * @param val значение для вывода
     * @return строка
     */
    public static String format(final double val) {
        String ret = DECIMAL_FORMAT.format(val);
        final int ind = ret.lastIndexOf(".000");
        if (ind > -1) {
            ret = ret.substring(0, ind);
        }
        return ret;
    }

    /**
     * Форматированный вывод десятичного числа.
     * @param val значение для вывода
     * @return строка
     */
    public static String formatLog(final double val) {
        return DECIMAL_FORMAT.format(val);
    }

}
