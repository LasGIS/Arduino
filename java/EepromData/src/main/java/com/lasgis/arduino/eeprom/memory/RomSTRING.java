/*
 *  @(#)RomSTRING.java  last: 25.05.2018
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
    int size() {
        return val.length();
    }

    @Override
    String define() {
        return "s";
    }

    @Override
    ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        final int count = val.length();
        return buff.put((byte) (count & 0xff)).put(val.getBytes(CHARSET));
    }
}
