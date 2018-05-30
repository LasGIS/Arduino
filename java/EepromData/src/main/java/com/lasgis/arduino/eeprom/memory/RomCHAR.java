/*
 *  @(#)RomCHAR.java  last: 30.05.2018
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
public class RomCHAR extends RomData {

    private char val;

    private RomCHAR(final String name, final char val) {
        super(name);
        this.val = val;
    }

    public static RomCHAR of(final char val) {
        return new RomCHAR(null, val);
    }

    public static RomCHAR of(final String name, final char val) {
        return new RomCHAR(name, val);
    }

    @Override
    int size() {
        return 1;
    }

    @Override
    String define() {
        return "c";
    }

    @Override
    public ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        setOffset(buff.position());
        return buff.put(Character.toString(val).getBytes(CHARSET));
    }
}
