/*
 *  @(#)MemoryRoms.java  last: 15.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

/**
 * The Class RomMemory содержит список загрузок в ROM (ПЗУ) + header file name.
 *
 * @author Vladimir Laskin
 * @since 15.02.2023 : 13:29
 */
@Data
@NoArgsConstructor(staticName = "of")
@AllArgsConstructor(staticName = "of")
public class MemoryRoms {
    private String headerFilename;
    private List<BatchMemory> list;
}
