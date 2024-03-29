/*
 *  @(#)RomEMPTY.java  last: 19.03.2023
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

/**
 * Массив простых элементов или объектов.
 * Массив должен состоять из одинаковых элементов,
 * поэтому нет смысла структуру элемента описывать в каждом элементе.
 * Однако в начале массива мы должны указать количество элементов массива.
 *
 * В EEPROM храним:
 * 1 - размер всего массива, включая первые 3 байта;
 * 2 - количество элементов массива;
 * 3 - телои самого массива (т.е. все его элементы по порядку).
 *
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
@ToString(callSuper = true)
public class RomEMPTY extends RomData {

    private RomEMPTY(final String name) {
        super(name, null);
    }

    public static RomEMPTY of() {
        return new RomEMPTY(null);
    }

    public static RomEMPTY of(final String name) {
        return new RomEMPTY(name);
    }

    @Override
    public int size() {
        return 0;
    }

    @Override
    public Character defChar() {
        return '_';
    }

    @Override
    public ByteArrayBuilder toEeprom(
        final ByteArrayBuilder buff,
        final int addressEeprom, final Map<String, AddressToRoms> reference2Address
    ) throws UnsupportedEncodingException {
        return buff;
    }
}
