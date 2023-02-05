/*
 *  @(#)DeviceWrap.java  last: 05.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.panels;

import lombok.Getter;
import lombok.RequiredArgsConstructor;

import javax.xml.bind.DatatypeConverter;

/**
 * The Class HexIntegerWrap definition.
 *
 * @author Vladimir Laskin
 * @since 04.02.2023 : 17:22
 */
@Getter
@RequiredArgsConstructor
public class DeviceWrap {
    private final byte device;

    /**
     * Конструктор
     *
     * @param device device as int
     * @return new DeviceWrap
     */
    public static DeviceWrap of(int device) {
        return new DeviceWrap((byte) device);
    }

    @Override
    public String toString() {
        final byte[] buf = {device};
        return "0x" + DatatypeConverter.printHexBinary(buf);
    }
}
