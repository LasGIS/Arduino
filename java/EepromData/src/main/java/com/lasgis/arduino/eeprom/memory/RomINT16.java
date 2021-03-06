/*
 * @(#)RomINT16.java
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright © 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import com.lasgis.util.ByteArrayBuilder;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.ToString;

import java.io.UnsupportedEncodingException;

/**
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
@ToString(callSuper = true)
public class RomINT16 extends RomData {

    private short val;

    private RomINT16(final String name, final short val) {
        super(name);
        this.val = val;
    }

    public static RomINT16 of(final short val) {
        return new RomINT16(null, val);
    }

    public static RomINT16 of(final String name, final short val) {
        return new RomINT16(name, val);
    }

    public static RomINT16 of(final int val) {
        return new RomINT16(null, (short) val);
    }

    public static RomINT16 of(final String name, final int val) {
        return new RomINT16(name, (short) val);
    }

    @Override
    public int size() {
        return Short.BYTES;
    }

    @Override
    public String define() {
        return "i";
    }

    @Override
    public ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        setOffset(buff.position());
        return buff.putShort(val);
    }
}
