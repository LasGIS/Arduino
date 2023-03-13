/*
 *  @(#)RomCHAR.java  last: 13.03.2023
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
public class RomCHAR extends RomData {

    private char val;

    private RomCHAR(final String name, final String refId, final char val) {
        super(name, refId);
        this.val = val;
    }

    public static RomCHAR of(final char val) {
        return new RomCHAR(null, null, val);
    }

    public static RomCHAR of(final String name, final String refId, final char val) {
        return new RomCHAR(name, refId, val);
    }

    @Override
    public int size() {
        return Byte.BYTES;
    }

    @Override
    public Character defChar() {
        return 'c';
    }

    @Override
    public ByteArrayBuilder toEeprom(
        final ByteArrayBuilder buff,
        final Map<String, AddressToRoms> reference2Address
    ) throws UnsupportedEncodingException {
        updateOffset(buff, reference2Address);
        return buff.put(Character.toString(val).getBytes(CHARSET));
    }
}
