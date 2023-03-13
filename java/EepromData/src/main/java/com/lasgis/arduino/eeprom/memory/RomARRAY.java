/*
 *  @(#)RomARRAY.java  last: 13.03.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import com.lasgis.arduino.eeprom.CommonInfoException;
import com.lasgis.util.ByteArrayBuilder;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.ToString;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Map;

/**
 * Массив простых элементов или объектов. Массив должен состоять из одинаковых элементов, поэтому нет смысла структуру элемента описывать в каждом
 * элементе. Однако в начале массива мы должны указать количество элементов массива.
 * <p>
 * В EEPROM храним: 1 - размер всего массива, включая первые 3 байта; 2 - количество элементов массива; 3 - телои самого массива (т.е. все его
 * элементы по порядку).
 *
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
@ToString(callSuper = true)
public class RomARRAY extends RomData {

    /** массив, содержащий элементы объекта */
    private ArrayList<RomData> array = new ArrayList<>();
    private String define;

    private RomARRAY(final String name, final String refId) {
        super(name, refId);
    }

    public static RomARRAY of() {
        return new RomARRAY(null, null);
    }

    public static RomARRAY of(final String name, final String refId) {
        return new RomARRAY(name, refId);
    }

    @Override
    public int size() {
        define = define();
        /* 1 - size block; 2 - size of definition char; 3 - count of array items */
        int size = Short.BYTES + Byte.BYTES + Short.BYTES;
        for (final RomData item : array) {
            size += item.size();
        }
        return size;
    }

    @Override
    public Character defChar() {
        return 'a';
    }

    public String define() {
        Character ret = array.stream().findAny().map(RomData::defChar).orElse(' ');
        return ret.toString();
    }

    @Override
    public ByteArrayBuilder toEeprom(
        final ByteArrayBuilder buff, final Map<String, AddressToRoms> reference2Address
    ) throws UnsupportedEncodingException {
        updateOffset(buff, reference2Address);
        buff.putShort(size() - Short.BYTES).put(define().getBytes(CHARSET)).putShort(array.size());
        for (final RomData item : array) {
            item.toEeprom(buff, reference2Address);
        }
        return buff;
    }

    public RomARRAY add(final RomData rom) {
        if (!array.isEmpty()) {
            final RomData first = array.get(0);
            if (!first.defChar().equals(rom.defChar())) {
                throw new CommonInfoException("Элементы массива должны быть одинаковыми!");
            }
        }
        array.add(rom);
        return this;
    }
}
