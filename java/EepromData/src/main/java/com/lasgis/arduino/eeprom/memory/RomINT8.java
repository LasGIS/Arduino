/*
 *  @(#)RomINT8.java  last: 25.05.2018
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
class RomINT8 extends RomData {

    private byte val;

    private RomINT8(final String name, final byte val) {
        super(name);
        this.val = val;
    }

    static RomINT8 of(final byte val) {
        return new RomINT8(null, val);
    }

    static RomINT8 of(final String name, final byte val) {
        return new RomINT8(name, val);
    }

    @Override
    int size() {
        return Byte.BYTES;
    }

    @Override
    ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        return buff.put(val);
    }
}
