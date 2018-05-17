/*
 *  @(#)UpLoadInfoException.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom;

/**
 * Для вывода без StackTrace
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
public class UpLoadInfoException extends RuntimeException {
    public UpLoadInfoException(final String message) {
        super(message);
    }
}
