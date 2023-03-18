/*
 *  @(#)RomINT32.java  last: 19.03.2023
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
public class RomINT32 extends RomData {

    private int val;

    public RomINT32(final String name, final String refId, final int val) {
        super(name, refId);
        this.val = val;
    }

    public static RomINT32 of(final int val) {
        return new RomINT32(null, null, val);
    }

    public static RomINT32 of(final String name, final String refId, final int val) {
        return new RomINT32(name, refId, val);
    }

    @Override
    public int size() {
        return Integer.BYTES;
    }

    @Override
    public Character defChar() {
        return 'l';
    }

    @Override
    public ByteArrayBuilder toEeprom(
        final ByteArrayBuilder buff,
        final int addressEeprom,
        final Map<String, AddressToRoms> reference2Address
    ) throws UnsupportedEncodingException {
        updateOffset(buff, addressEeprom, reference2Address);
        return buff.putInt(val);
    }
}
