/*
 *  @(#)RomSTRING.java  last: 13.03.2023
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
public class RomSTRING extends RomData {

    private String val;

    private RomSTRING(final String name, final String refId, final String val) {
        super(name, refId);
        this.val = val;
    }

    public static RomSTRING of(final String val) {
        return new RomSTRING(null, null, val);
    }

    public static RomSTRING of(final String name, final String refId, final String val) {
        return new RomSTRING(name, refId, val);
    }

    @Override
    public int size() {
        /* 1 - size of string; 2 - string itself */
        return Short.BYTES + val.length();
    }

    @Override
    public Character defChar() {
        return 's';
    }

    @Override
    public ByteArrayBuilder toEeprom(
        final ByteArrayBuilder buff,
        final Map<String, AddressToRoms> reference2Address
    ) throws UnsupportedEncodingException {
        updateOffset(buff, reference2Address);
        return buff.putShort(size() - Short.BYTES).put(val.getBytes(CHARSET));
    }
}
