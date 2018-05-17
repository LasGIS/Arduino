/*
 *  @(#)RomDOUBLE.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.Data;
import lombok.EqualsAndHashCode;

import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;

/**
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
public class RomDOUBLE extends RomData {

    private double val;
    
    public RomDOUBLE(final String name) {
        super(name);
    }

    private RomDOUBLE(final String name, final double val) {
        super(name);
        this.val = val;
    }

    static RomDOUBLE of(final double val) {
        return new RomDOUBLE(null, val);
    }

    static RomDOUBLE of(final String name, final double val) {
        return new RomDOUBLE(name, val);
    }

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        byte[] bytes = new byte[8];
        ByteBuffer.wrap(bytes).putDouble(val);
        return bytes;
    }
}
