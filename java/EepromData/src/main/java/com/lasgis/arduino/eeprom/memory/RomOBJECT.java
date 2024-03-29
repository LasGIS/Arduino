/*
 *  @(#)RomOBJECT.java  last: 19.03.2023
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
import java.util.ArrayList;
import java.util.Map;

/**
 * Объект структуры или класса. Структура имеет разные элементы. Если класс состоит из простых элементов, то размер класса равен размеру в EEPROM Если
 * класс содержит строки или другие структуры, то при десериализации надо создавать в памяти объект строки, а в класс ложить указатель.
 * <p>
 * В EEPROM храним: 1 - размер всего объекта, включая первые 3 байта; 2 - количество элементов объекта; 3 - тело самого объекта (т.е. все его элементы
 * по порядку). Определение структуры определяется по элементам объекта.
 *
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
@ToString(callSuper = true)
public class RomOBJECT extends RomData {

    /** массив, содержащий элементы объекта */
    private ArrayList<RomData> array = new ArrayList<>();
    private String define;

    private RomOBJECT(final String name, final String refId) {
        super(name, refId);
    }

    public static RomOBJECT of() {
        return new RomOBJECT(null, null);
    }

    public static RomOBJECT of(final String name, final String refId) {
        return new RomOBJECT(name, refId);
    }

    @Override
    public int size() {
        define = define();
        /* 1 - size block; 2 - size of definition string; 3 - definition string itself */
        int size = Short.BYTES + Short.BYTES + define.length();
        for (final RomData item : array) {
            size += item.size();
        }
        return size;
    }

    @Override
    public Character defChar() {
        return 'o';
    }

    public String define() {
        final StringBuilder sb = new StringBuilder();
        for (final RomData item : array) {
            sb.append(item.defChar());
        }
        return sb.toString();
    }

    @Override
    public ByteArrayBuilder toEeprom(
        final ByteArrayBuilder buff,
        final int addressEeprom,
        final Map<String, AddressToRoms> reference2Address
    ) throws UnsupportedEncodingException {
        updateOffset(buff, addressEeprom, reference2Address);
        buff.putShort(size() - Short.BYTES).putShort(define.length()).put(define.getBytes(CHARSET));
        for (final RomData item : array) {
            item.toEeprom(buff, addressEeprom, reference2Address);
        }
        return buff;
    }

    public RomOBJECT add(final RomData rom) {
        array.add(rom);
        return this;
    }
}
