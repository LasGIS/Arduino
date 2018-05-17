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
 * Структура имеет разные элементы.
 * Если класс состоит из простых элементов, то размер класса равен размеру в EEPROM
 * Если класс содержит строки или другие структуры, то при десериализации надо
 * создавать в памяти объект строки, а в класс ложить указатель.
 *
 * Поэтому в EEPROM надо хранить как размер объекта, так и описание его структуры.
 * Размер объекта вычисляется при создании EEPROM образа.
 * Описание структуры определяется по элементам объекта.
 *
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
public class RomOBJECT extends RomData {

    /** массив, содержащий элементы объекта */
    private ArrayList<RomData> array = new ArrayList<>();
//    private short size;

    public RomOBJECT(final String name) {
        super(name);
    }

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        return new byte[0];
    }
}
