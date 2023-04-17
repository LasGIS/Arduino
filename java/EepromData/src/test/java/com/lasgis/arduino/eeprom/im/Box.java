/*
 *  @(#)Box.java  last: 16.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.im;

// обозначение типа - T

import lombok.Data;

/**
 * The Class Box definition.
 * @param <T> generic T
 *
 * @author Vladimir Laskin
 * @since 16.04.2023 : 12:07
 */
@Data
@MyAnnotation("class (ElementType.TYPE)")
public final class Box<T> {
    @MyAnnotation(value = "field (ElementType.FIELD)")
    private final T item;
    @MyAnnotation("(ElementType.CONSTRUCTOR)")
    public Box(final T item) {
        this.item = item;
    }
    @MyAnnotation("ElementType.METHOD")
    void print() {
        System.out.printf("item = %n\n", item);
    }
}
