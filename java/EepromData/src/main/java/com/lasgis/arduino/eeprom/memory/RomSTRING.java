/*
 *  @(#)RomSTRING.java  last: 17.05.2018
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
class RomSTRING extends RomData {

    private String val;

    private RomSTRING(final String name, final String val) {
        super(name);
        this.val = val;
    }

    static RomSTRING of(final String val) {
        return new RomSTRING(null, val);
    }

    static RomSTRING of(final String name, final String val) {
        return new RomSTRING(name, val);
    }

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        final int count = val.length();
        final byte[] str = val.getBytes(CHARSET);
        final byte[] out = new byte[count + 1];
        out[0] = (byte) (count & 0xff);
        for (int i = 0, j = 1; i < count; i++, j++) {
            out[j] = str[i];
        }
        return out;
    }
}
