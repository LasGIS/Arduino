/*
 *  @(#)RomINT16.java  last: 12.03.2023
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
import java.util.Map;

/**
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
@ToString(callSuper = true)
public class RomINT16 extends RomData {

    private short val;

    private RomINT16(final String name, final String refId, final short val) {
        super(name, refId);
        this.val = val;
    }

    public static RomINT16 of(final short val) {
        return new RomINT16(null, null, val);
    }

    public static RomINT16 of(final String name, final String refId, final short val) {
        return new RomINT16(name, refId, val);
    }

    public static RomINT16 of(final int val) {
        return new RomINT16(null, null, (short) val);
    }

    public static RomINT16 of(final String name, final String refId, final int val) {
        return new RomINT16(name, refId, (short) val);
    }

    @Override
    public int size() {
        return Short.BYTES;
    }

    @Override
    public Character defChar() {
        return 'i';
    }

    @Override
    public ByteArrayBuilder toEeprom(
        final ByteArrayBuilder buff,
        final Map<String, AddressToRoms> reference2Address
    ) throws UnsupportedEncodingException {
        updateOffset(buff, reference2Address);
        return buff.putShort(val);
    }
}
