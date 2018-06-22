/*
 * @(#)RomEMPTY.java
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright © 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import com.lasgis.arduino.eeprom.CommonInfoException;
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
public class RomEMPTY extends RomData {

    private RomEMPTY(final String name) {
        super(name);
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
    public String define() {
        return "";
    }

    @Override
    public ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        return buff;
    }
}
