/*
 *  @(#)UpLoadInfoException.java  last: 30.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom;

import java.text.MessageFormat;

/**
 * Для вывода без StackTrace
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
public class CommonInfoException extends RuntimeException {
    public CommonInfoException(final String message) {
        super(message);
    }

    public CommonInfoException(final String message, final Object... args) {
        super(MessageFormat.format(message, args));
    }
}
