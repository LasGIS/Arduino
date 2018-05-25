/*
 *  @(#)RomARRAY.java  last: 25.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import com.lasgis.arduino.eeprom.UpLoadInfoException;
import com.lasgis.util.ByteArrayBuilder;
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

    /** массив, содержащий элементы объекта */
    private ArrayList<RomData> array = new ArrayList<>();

    private RomARRAY(final String name) {
        super(name);
    }

    public static RomARRAY of() {
        return new RomARRAY(null);
    }

    public static RomARRAY of(final String name) {
        return new RomARRAY(name);
    }

    @Override
    int size() {
        int size = 0;
        for (final RomData item : array) {
            size += item.size();
        }
        return size;
    }

    @Override
    ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        buff.put((byte) array.size());
        for (final RomData item : array) {
            item.toEeprom(buff);
        }
        return buff;
    }

    RomARRAY add(final RomData rom) {
        if (!array.isEmpty()) {
            final RomData first = array.get(0);
            if (first.size() != rom.size()) {
                throw new UpLoadInfoException("Элементы массива должны быть одинаковыми!");
            }
        }
        array.add(rom);
        return this;
    }
}
