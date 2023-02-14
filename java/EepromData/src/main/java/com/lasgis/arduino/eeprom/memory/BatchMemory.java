/*
 *  @(#)BatchMemory.java  last: 15.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.ArrayList;
import java.util.List;

/**
 * The Class MemoryRoms definition.
 *
 * @author Vladimir Laskin
 * @since 15.02.2023 : 1:06
 */
@Data
@NoArgsConstructor(staticName = "of")
public class BatchMemory {
    private List<RomData> romDataList = new ArrayList<>();
    private byte device;
    private int offset;
    private String prefix;
}
