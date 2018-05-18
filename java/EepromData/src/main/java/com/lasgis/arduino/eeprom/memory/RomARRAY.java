/*
 *  @(#)RomARRAY.java  last: 18.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.Data;
import lombok.EqualsAndHashCode;

import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
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
    byte[] toByte() throws UnsupportedEncodingException {
        final ByteBuffer buff = ByteBuffer.allocate(30);
        buff.put((byte) array.size());
        for (final RomData item : array) {
            buff.put(item.toByte());
        }
        final int position = buff.position();
        final byte[] out = new byte[position];
        buff.position(0);
        buff.get(out);
        return out;
    }

    RomARRAY add(final RomData rom) {
        array.add(rom);
        return this;
    }
}
