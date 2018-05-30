/*
 *  @(#)RomData.java  last: 30.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import com.lasgis.util.ByteArrayBuilder;
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
 * 3 При определении структуры создается строка, где первый байт
 *   определяет размер строки, например:
 *   |057B6362617D| "_{cib}" - OBJECT, состоящий из CHAR, INT16, INT8
 *   |00| "" - пустое описание (без имени)
 * 4 Для простых объектов размер известен заранее. Для сложных (STRING,
 *   OBJECT, ARRAY) размер пишется в начале объекта (Short два байта).
 *   Кроме этого для OBJECT и ARRAY третий байт задаёт количество подобъектов.
 *
 * Правила deserialization:
 * Для десериализации нужно знать структуру EEPROM образа.
 * Адрес EEPROM образа всегда загружается в #define, а затем используется,
 * по необходимости в программе arduino.
 * Есть следующие варианты, где хранить определения:
 * 1 Определение структуры хранить в EEPROM для каждого объекта (включая byte)
 *   Это загрузит EEPROM, но освободит PROGMEM.
 * 2 Определение структуры хранить в EEPROM только для именованного объекта
 *   Здесь при чтении объекта, надо предварительно прочитать определение
 *   структуры в ОЗУ, а затем запустить десериализатор по этому определению.
 *   Однако, в этом случае мы должны как-то определять: есть в начале
 *   EEPROM объекта определитель или нет его...
 *   Как вариант, можно запретить делать именованные объекты внутри
 *   именованного EEPROM объекта (т.е. Один объект - одно имя).
 * 3 Определение структуры хранить в программе (доступ через PROGMEM), но
 *   только для именованных объектов.
 *   Определение загружается в #define и при необходимости вызывается для
 *   именованной десериализации.
 * Будем реализовывать 3-тий вариант
 *
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Data
public abstract class RomData {

    protected static final Charset CHARSET = Charset.forName("windows-1251");

    private String name;
    private int offset = 0;

    public RomData(String name) {
        this.name = name;
        this.offset = 0;
    }

    /** размер образа объекта в EEPROM */
    abstract int size();

    /**
     * Вернуть определение структуры. Структура определяется однобайтовым знаком:
     *   CHAR   - 'c'
     *   INT8   - 'b'
     *   INT16  - 'i'
     *   INT32  - 'l'
     *   DOUBLE - 'd'
     *   FLOAT  - 'f'
     *   STRING - 's'
     *   OBJECT - '{}'
     *   ARRAY  - '[]'
     * @return определение структуры
     */
    abstract String define();

    /**
     * Добавляем образ объекта в накопительный массив байт.
     * @param buff накопительный объект ByteArrayBuilder
     * @return накопительный объект ByteArrayBuilder
     * @throws UnsupportedEncodingException если ошибка
     */
    public abstract ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException;

    /**
     * Получаем образ объекта в массив байт.
     * В дальнейшем upload образ в arduino.
     * @return массив байт
     * @throws UnsupportedEncodingException если ошибка
     */
    public byte[] toEeprom() throws UnsupportedEncodingException {
        return toEeprom(new ByteArrayBuilder()).toByte();
    }
}
