/*
 *  @(#)AddressToRoms.java  last: 12.03.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import com.lasgis.util.ByteArrayBuilder;
import lombok.AccessLevel;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.ArrayList;
import java.util.List;

/**
 * Накопление объектов {@link RomADDRESS} и установление самого адреса.
 *
 * @author Vladimir Laskin
 * @since 12.03.2023 : 0:05
 */
@Data
@NoArgsConstructor
public class AddressToRoms {
    private String refId;
    private Integer offset;
    /** адреса для установки ссылки */
    @Setter(AccessLevel.NONE)
    private List<RomADDRESS> romAddresses = new ArrayList<>();

    public static AddressToRoms of(final String refId) {
        final AddressToRoms inst = new AddressToRoms();
        inst.refId = refId;
        return inst;
    }

    void addRom(final RomADDRESS rom) {
        romAddresses.add(rom);
    }

    public void enrich(final ByteArrayBuilder buff) {
        for (final RomADDRESS rom : romAddresses) {
            rom.setReference(offset);
            buff.putShort(rom.getOffset(), offset);
        }
    }
}
