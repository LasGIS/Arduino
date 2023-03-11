/*
 *  @(#)RomADDRESS.java  last: 12.03.2023
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
import java.util.Objects;

/**
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
@ToString(callSuper = true)
public class RomADDRESS extends RomData {

    private Integer reference = null;

    private RomADDRESS(final String name, final String refId) {
        super(name, refId);
    }

    public static RomADDRESS of(final String reference) {
        return new RomADDRESS(null, reference);
    }

    public static RomADDRESS of(final String name, final String reference) {
        return new RomADDRESS(name, reference);
    }

    @Override
    public int size() {
        return Short.BYTES;
    }

    @Override
    public Character defChar() {
        return 'r';
    }

    @Override
    public ByteArrayBuilder toEeprom(
        final ByteArrayBuilder buff,
        final Map<String, AddressToRoms> reference2Address
    ) throws UnsupportedEncodingException {
        updateOffset(buff, reference2Address);
        return buff.putShort(Objects.nonNull(reference) ? reference : 0xffff);
    }
}
