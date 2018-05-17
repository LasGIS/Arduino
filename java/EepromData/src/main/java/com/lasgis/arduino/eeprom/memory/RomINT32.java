/*
 *  @(#)RomINT32.java  last: 17.05.2018
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
public class RomINT32 extends RomData {

    private int val;

    public RomINT32(final String name, final int val) {
        super(name);
        this.val = val;
    }

    static RomINT32 of(final int val) {
        return new RomINT32(null, val);
    }

    static RomINT32 of(final String name, final int val) {
        return new RomINT32(name, val);
    }

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        final byte[] out = new byte[4];
        int valOut = val;
        for (int i = 0; i < 4; i++) {
            out[i] = (byte) (valOut & 0xff);
            valOut >>= 8;
        }
        return out;
    }
}
