/*
 *  @(#)RomINT8.java  last: 12.03.2023
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
public class RomINT8 extends RomData {

    private byte val;

    private RomINT8(final String name, final String refId, final byte val) {
        super(name, refId);
        this.val = val;
    }

    public static RomINT8 of(final byte val) {
        return new RomINT8(null, null, val);
    }

    public static RomINT8 of(final String name, final String refId, final byte val) {
        return new RomINT8(name, refId, val);
    }

    public static RomINT8 of(final int val) {
        return new RomINT8(null, null, (byte) val);
    }

    public static RomINT8 of(final String name, final String refId, final int val) {
        return new RomINT8(name, refId, (byte) val);
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
    public ByteArrayBuilder toEeprom(final ByteArrayBuilder buff, final Map<String, AddressToRoms> reference2Address) throws UnsupportedEncodingException {
        updateOffset(buff, reference2Address);
        return buff.put(val);
    }
}
