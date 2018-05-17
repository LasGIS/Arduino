/*
 *  @(#)RomData.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.AllArgsConstructor;
import lombok.Data;

import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;

/**
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Data
@AllArgsConstructor
public abstract class RomData {

    protected static final Charset CHARSET = Charset.forName("windows-1251");

    private String name;

    abstract byte[] toByte() throws UnsupportedEncodingException;
}
