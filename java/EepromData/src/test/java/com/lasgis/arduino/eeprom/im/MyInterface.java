/*
 *  @(#)MyInterface.java  last: 16.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.im;

/**
 * The Class MyInterface definition.
 *
 * @author Vladimir Laskin
 * @since 16.04.2023 : 17:23
 */
public interface MyInterface {
    default int doSomething() {
        return 0;
    }
}
