/*
 *  @(#)RomOBJECT.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.Data;
import lombok.EqualsAndHashCode;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

/**
 * Объект структуры или класса.
 * Следующие моменты:
 * 1 -
 *
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
public class RomOBJECT extends RomData {

    private ArrayList<RomData> array = new ArrayList<>();

    public RomOBJECT(final String name) {
        super(name);
    }

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        return new byte[0];
    }
}
