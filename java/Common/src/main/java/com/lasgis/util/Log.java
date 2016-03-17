/**
 * @(#)Log.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2015 LasGIS Company. All Rights Reserved.
 */
package com.lasgis.util;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Класс для вывода в лог.
 * @author VLaskin
 * @since 08.03.2006 21:32:06
 * @version 1.0
 */
public class Log {

    /**
     * Скрытый конструктор.
     */
    private Log() {
    }

    /**
     * Трассировка ошибки в log.
     * @param client of logger.
     * @param exc Exception
     */
    public static void stackTrace(Class client, Throwable exc) {
        Logger logger = LoggerFactory.getLogger(client);
        StackTraceElement[] stack = exc.getStackTrace();
        logger.error(
            exc.getClass().getName() + ": "
            + exc.getMessage() + "\r\n"
        );
        StringBuffer sb = new StringBuffer();
        for (StackTraceElement aStack : stack) {
            sb.append("\tat ")
                .append(aStack)
                .append("\r\n");
        }
        logger.error(sb.toString());
    }

}
