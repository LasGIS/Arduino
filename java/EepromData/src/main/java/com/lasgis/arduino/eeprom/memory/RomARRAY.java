/*
 *  @(#)RomARRAY.java  last: 17.05.2018
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
 * Массив простых элементов или объектов.
 * Массив должен состоять из одинаковых элементов,
 * поэтому нет смысла структуру элемента описывать в каждом элементе.
 * Однако в начале массива мы должны указать количество элементов массива.
 *
 * В EEPROM храним количество элементов массива и описание структуры одного элемента.
 *
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
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
