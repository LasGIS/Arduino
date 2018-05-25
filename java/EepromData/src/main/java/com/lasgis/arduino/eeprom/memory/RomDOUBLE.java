/*
 *  @(#)RomDOUBLE.java  last: 25.05.2018
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
    int size() {
        return Double.BYTES;
    }

    @Override
    ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        return buff.putDouble(val);
    }
}
