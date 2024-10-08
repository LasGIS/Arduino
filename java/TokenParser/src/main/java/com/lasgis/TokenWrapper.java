/*
 *  @(#)TokenWrapper.java  last: 06.10.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis;

import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * @author Vladimir Laskin
 * @since 22.06.2018
 */
@Getter
@AllArgsConstructor(staticName = "of")
public class TokenWrapper<Type> {
    private Type data;
    private Token token;
}
