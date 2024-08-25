/*
 *  @(#)CommonInfoException.java  last: 25.08.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont;

import java.text.MessageFormat;

/**
 * Для вывода без StackTrace
 *
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
public class CommonInfoException extends RuntimeException {
    /**
     * Вывод сообщения
     *
     * @param message сообщение
     */
    public CommonInfoException(final String message) {
        super(message);
    }

    /**
     * Вывод сообщения с параметрами
     *
     * @param message сообщение
     * @param args    параметры
     */
    public CommonInfoException(final String message, final Object... args) {
        super(MessageFormat.format(message, args));
    }
}
