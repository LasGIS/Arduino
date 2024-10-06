/*
 *  @(#)KeywordTypeCpp.java  last: 06.10.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.load;

import com.lasgis.KeywordType;
import lombok.Getter;

import static java.util.Objects.nonNull;

/**
 * Типы данных
 *
 * @author Vladimir Laskin
 * @since 06.10.2024 : 17:49
 */
@Getter
public enum KeywordTypeCpp implements KeywordType {
    UNDEFINED(null),
    /** Структурные определители. */
    INCLUDE("include"),
    DEFINE("define"),
    PROGMEM("PROGMEM"),
    CONST("const"),
    UNSIGNED("unsigned"),
    CHAR("char"),
    EXTERN("extern");

    final String name;

    KeywordTypeCpp(final String name) {
        this.name = name;
    }

    /**
     * @param value значение
     * @return получаем тип по значению
     */
    public KeywordType of(final String value) {
        for (KeywordTypeCpp type : KeywordTypeCpp.values()) {
            final String name = type.getName();
            if (nonNull(name) && name.equals(value)) {
                return type;
            }
        }
        return null;
    }
}


