/*
 *  @(#)RomINT8.java  last: 08.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
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
public class RomINT8 extends RomData {

    private byte val;

    private RomINT8(final String name, final byte val) {
        super(name);
        this.val = val;
    }

    public static RomINT8 of(final byte val) {
        return new RomINT8(null, val);
    }

    public static RomINT8 of(final String name, final byte val) {
        return new RomINT8(name, val);
    }

    public static RomINT8 of(final int val) {
        return new RomINT8(null, (byte) val);
    }

    public static RomINT8 of(final String name, final int val) {
        return new RomINT8(name, (byte) val);
    }

    @Override
    public int size() {
        return Byte.BYTES;
    }

    @Override
    public Character defChar() {
        return 'b';
    }

    @Override
    public ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        setOffset(buff.position());
        return buff.put(val);
    }
}
