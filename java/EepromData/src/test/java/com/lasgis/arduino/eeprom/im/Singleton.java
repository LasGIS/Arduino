/*
 *  @(#)Singleton.java  last: 15.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.im;

/**
 * The Class Singleton definition.
 *
 * @author Vladimir Laskin
 * @since 15.04.2023 : 17:49
 */
public class Singleton {
    private static Singleton instance;
    private Singleton() {
        // . . .
    }
    public static synchronized Singleton getInstance() {
        if (instance == null) {
            synchronized (Singleton.class) {
                instance = new Singleton();
            }
        }
        return instance;
    }
}
