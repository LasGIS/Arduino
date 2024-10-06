/*
 *  @(#)KeywordTypeQuasiCpp.java  last: 06.10.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load;

import com.lasgis.KeywordType;
import lombok.Getter;

import static java.util.Objects.nonNull;

/**
 * Типы данных quasiCpp
 *
 * @author Vladimir Laskin
 * @since 06.10.2024 : 17:49
 */
@Getter
public enum KeywordTypeQuasiCpp implements KeywordType {
    UNDEFINED(null),
    /** Структурные определители. */
    MEMORY_ROMS("MemoryRoms"),
    BATCH_MEMORY("BatchMemory"),
    /** Типы данных. */
    CHAR("char"),
    INT8("int8"),
    INT16("int16"),
    INT32("int32"),
    FLOAT("float"),
    STRING("string"),
    DUMP("dump"),
    EEPROM_ADDRESS("eepromAddress"),
    OBJECT("object"),
    ARRAY("array");

    final String name;

    KeywordTypeQuasiCpp(final String name) {
        this.name = name;
    }

    /**
     * @param value значение
     * @return получаем тип по значению
     */
    public KeywordType of(final String value) {
        for (KeywordTypeQuasiCpp type : KeywordTypeQuasiCpp.values()) {
            final String name = type.getName();
            if (nonNull(name) && name.equals(value)) {
                return type;
            }
        }
        return null;
    }
}


