/*
 *  @(#)RomARRAY.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;

/**
 * Массив простых элементов или объектов.
 *
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
public class RomARRAY extends RomData {

    private ArrayList<RomData> array = new ArrayList<>();

    public RomARRAY(final String name) {
        super(name);
    }

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        return new byte[0];
    }
}
