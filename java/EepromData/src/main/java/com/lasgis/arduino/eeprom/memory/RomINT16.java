/*
 *  @(#)RomINT16.java  last: 17.05.2018
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
class RomINT16 extends RomData {

    private short val;

    private RomINT16(final String name, final short val) {
        super(name);
        this.val = val;
    }

    static RomINT16 of(final short val) {
        return new RomINT16(null, val);
    }

    static RomINT16 of(final String name, final short val) {
        return new RomINT16(name, val);
    }

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        final byte[] out = new byte[2];
        short valOut = val;
        for (int i = 0; i < 2; i++) {
            out[i] = (byte) (valOut & 0xff);
            valOut >>= 8;
        }
        return out;
    }
}
