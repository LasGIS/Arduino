/*
 *  @(#)RomDOUBLE.java  last: 19.03.2023
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
 * @since 17.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
@ToString(callSuper = true)
public class RomDOUBLE extends RomData {

    private double val;

    private RomDOUBLE(final String name, final String refId, final double val) {
        super(name, refId);
        this.val = val;
    }

    public static RomDOUBLE of(final double val) {
        return new RomDOUBLE(null, null, val);
    }

    public static RomDOUBLE of(final String name, final String refId, final double val) {
        return new RomDOUBLE(name, refId, val);
    }

    @Override
    public int size() {
        return Double.BYTES;
    }

    @Override
    public Character defChar() {
        return 'd';
    }

    @Override
    public ByteArrayBuilder toEeprom(
        final ByteArrayBuilder buff,
        final int addressEeprom,
        final Map<String, AddressToRoms> reference2Address
    ) throws UnsupportedEncodingException {
        updateOffset(buff, addressEeprom, reference2Address);
        return buff.putDouble(val);
    }
}
