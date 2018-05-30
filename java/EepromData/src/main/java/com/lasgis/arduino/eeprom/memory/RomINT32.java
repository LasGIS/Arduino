/*
 *  @(#)RomINT32.java  last: 30.05.2018
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
public class RomINT32 extends RomData {

    private int val;

    public RomINT32(final String name, final int val) {
        super(name);
        this.val = val;
    }

    public static RomINT32 of(final int val) {
        return new RomINT32(null, val);
    }

    public static RomINT32 of(final String name, final int val) {
        return new RomINT32(name, val);
    }

    @Override
    int size() {
        return Integer.BYTES;
    }

    @Override
    String define() {
        return "l";
    }

    @Override
    public ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        setOffset(buff.position());
        return buff.putInt(val);
    }
}
