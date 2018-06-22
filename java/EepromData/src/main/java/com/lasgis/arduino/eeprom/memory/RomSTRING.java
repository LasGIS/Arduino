/*
 * @(#)RomSTRING.java
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright © 2018, LasGIS Company. All Rights Reserved.
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
public class RomSTRING extends RomData {

    private String val;

    private RomSTRING(final String name, final String val) {
        super(name);
        this.val = val;
    }

    public static RomSTRING of(final String val) {
        return new RomSTRING(null, val);
    }

    public static RomSTRING of(final String name, final String val) {
        return new RomSTRING(name, val);
    }

    @Override
    public int size() {
        return val.length() + 2;
    }

    @Override
    public String define() {
        return "s";
    }

    @Override
    public ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        setOffset(buff.position());
        return buff.putShort(size()).put(val.getBytes(CHARSET));
    }
}
