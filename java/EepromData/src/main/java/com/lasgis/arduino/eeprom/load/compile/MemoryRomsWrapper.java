/*
 *  @(#)MemoryRomsWrapper.java  last: 11.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load.compile;

import com.lasgis.arduino.eeprom.memory.MemoryRoms;
import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * @author Vladimir Laskin
 * @since 22.06.2018
 */
@Getter
@AllArgsConstructor(staticName = "of")
public class MemoryRomsWrapper {
    private MemoryRoms data;
    private TokenParser.Token token;
}
