/*
 *  @(#)RomData.java  last: 25.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import com.lasgis.util.ByteArrayBuilder;
import lombok.AllArgsConstructor;
import lombok.Data;

import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;

/**
 * Правила сериализации:
 * 1 Объекты могут быть именованные и не именованные.
 *    если объект имеет имя, то мы должны указать его структуру и добавить
 *     #define в .H файл.
 *    если без имени, то структуру опускаем
 * 2 структура определяется однобайтовым знаком:
 *   CHAR   - 'c'
 *   INT8   - 'b'
 *   INT16  - 'i'
 *   INT32  - 'l'
 *   DOUBLE - 'd'
 *   FLOAT  - 'f'
 *   STRING - 's'
 *   OBJECT - '{}'
 *   ARRAY  - '[]'
 * 3 при описании структуры в EEPROM образ добавляется строка, где первый байт
 *   определяет размер строки, например:
 *   |057B6362617D| "_{cib}" - OBJECT, состоящий из CHAR, INT16, INT8
 *   |00| "" - пустое описание (без имени)
 *
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Data
@AllArgsConstructor
public abstract class RomData {

    protected static final Charset CHARSET = Charset.forName("windows-1251");

    private String name;

    /** размер образа объекта в EEPROM */
    abstract int size();

    /**
     * Вернуть описание структуры. Структура определяется однобайтовым знаком:
     *   CHAR   - 'c'
     *   INT8   - 'b'
     *   INT16  - 'i'
     *   INT32  - 'l'
     *   DOUBLE - 'd'
     *   FLOAT  - 'f'
     *   STRING - 's'
     *   OBJECT - '{}'
     *   ARRAY  - '[]'
     * @return описание структуры
     */
    abstract String define();

    /**
     * Добавляем образ объекта в накопительный массив байт.
     * @param buff накопительный объект ByteArrayBuilder
     * @return накопительный объект ByteArrayBuilder
     * @throws UnsupportedEncodingException если ошибка
     */
    abstract ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException;

    /**
     * Получаем образ объекта в массив байт.
     * В дальнейшем upload образ в arduino.
     * @return массив байт
     * @throws UnsupportedEncodingException если ошибка
     */
    byte[] toEeprom() throws UnsupportedEncodingException {
        return toEeprom(new ByteArrayBuilder()).toByte();
    }
}
