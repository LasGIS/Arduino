/*
 *  @(#)RomINT16.java  last: 25.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import com.lasgis.util.ByteArrayBuilder;
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
    int size() {
        return Short.BYTES;
    }

    @Override
    ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        return buff.putShort(val);
    }
}
