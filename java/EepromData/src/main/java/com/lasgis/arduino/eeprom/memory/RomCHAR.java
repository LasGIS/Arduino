/*
 *  @(#)RomCHAR.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.Data;
import lombok.EqualsAndHashCode;

import java.io.UnsupportedEncodingException;

/**
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
class RomCHAR extends RomData {

    private char val;

    private RomCHAR(final String name, final char val) {
        super(name);
        this.val = val;
    }

    static RomCHAR of(final char val) {
        return new RomCHAR(null, val);
    }

    static RomCHAR of(final String name, final char val) {
        return new RomCHAR(name, val);
    }

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        return Character.toString(val).getBytes(CHARSET);
    }
}
