/*
 *  @(#)CppOperator.java  last: 06.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.load.data;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * The Class RomMemory содержит список загрузок в ROM (ПЗУ) + header file name.
 *
 * @author Vladimir Laskin
 * @since 15.02.2023 : 13:29
 */
@Data
@NoArgsConstructor(staticName = "of")
@AllArgsConstructor(staticName = "of")
public class CppOperator {
    private TypeOperator type;
    private String name;
    private String value;
}
