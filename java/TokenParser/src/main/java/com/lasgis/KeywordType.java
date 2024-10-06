/*
 *  @(#)KeywordType.java  last: 06.10.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis;

/**
 * The Class KeywordType definition.
 *
 * @author Vladimir Laskin
 * @since 06.10.2024 : 17:22
 */
public interface KeywordType {
    String getName();

    KeywordType of(String name);
}
