/*
 *  @(#)RomData.java  last: 12.03.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import com.lasgis.util.ByteArrayBuilder;
import lombok.AccessLevel;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.Setter;
import lombok.ToString;
import lombok.extern.slf4j.Slf4j;

import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;
import java.util.HashMap;
import java.util.Map;

import static java.util.Objects.isNull;
import static java.util.Objects.nonNull;

/**
 * ROM (Read-only memory) - ПЗУ
 * <pre>Правила сериализации:
 * 1 Объекты могут быть именованные и не именованные.
 *    если объект имеет имя, то мы должны указать его структуру и добавить
 *     #define в .H файл.
 *    если без имени, то структуру опускаем
 * 2 структура определяется одно байтовым знаком:
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
 * </pre>
 *
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Slf4j
@Data
@NoArgsConstructor
@ToString(exclude = {"offset"})
public abstract class RomData {
    /** Charset. */
    public static final Charset CHARSET = Charset.forName("windows-1251");

    private String name;
    private String refId;
    @Setter(AccessLevel.NONE)
    private int offset = 0;

    /**
     * Конструктор.
     *
     * @param name имя объекта
     */
    public RomData(final String name, final String refId) {
        this.name = name;
        this.refId = refId;
        this.offset = 0;
    }

    public void updateOffset(final ByteArrayBuilder buff, final Map<String, AddressToRoms> reference2Address) {
        offset = buff.position();
        if (nonNull(refId)) {
            AddressToRoms addressToRoms = reference2Address.get(refId);
            if (isNull(addressToRoms)) {
                addressToRoms = AddressToRoms.of(refId);
                reference2Address.put(refId, addressToRoms);
            }
            if (this instanceof RomADDRESS) {
                addressToRoms.addRom((RomADDRESS) this);
            } else {
                if (nonNull(addressToRoms.getOffset())) {
                    log.error("Такой reference \"{}\" уже зарезервирован! Объект name: {}", refId, name);
                }
                addressToRoms.setOffset(offset);
            }
        }
    }

    /**
     * @return размер образа объекта в EEPROM
     */
    public abstract int size();

    /**
     * <pre>
     * Вернуть определение структуры. Структура определяется одно байтовым знаком:
     *   CHAR   - 'c'
     *   INT8   - 'b'
     *   INT16  - 'i'
     *   INT32  - 'l'
     *   DOUBLE - 'd'
     *   FLOAT  - 'f'
     *   STRING - 's'
     *   OBJECT - '{}' ('o')
     *   ARRAY  - '[]' ('a')
     *   </pre>
     *
     * @return определение структуры
     */
    public abstract Character defChar();

    public String define() {
        return "";
    }

    /**
     * Добавляем образ объекта в накопительный массив байт.
     *
     * @param buff              накопительный объект ByteArrayBuilder
     * @param reference2Address связь refId на реальный адрес в EEPROM
     * @return накопительный объект ByteArrayBuilder
     * @throws UnsupportedEncodingException если ошибка
     */
    public abstract ByteArrayBuilder toEeprom(
        final ByteArrayBuilder buff,
        final Map<String, AddressToRoms> reference2Address
    ) throws UnsupportedEncodingException;

    /**
     * <pre>
     * Получаем образ объекта в массив байт.
     * В дальнейшем upload образ в arduino.
     * </pre>
     *
     * @return массив байт
     * @throws UnsupportedEncodingException если ошибка
     */
    public byte[] toEeprom() throws UnsupportedEncodingException {
        final ByteArrayBuilder bab = new ByteArrayBuilder();
        final Map<String, AddressToRoms> reference2Address = new HashMap<>();
        return toEeprom(bab, reference2Address).toByte();
    }
}
