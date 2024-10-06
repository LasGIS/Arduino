/*
 *  @(#)BatchMemoryWrapper.java  last: 06.10.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load.compile;

import com.lasgis.Token;
import com.lasgis.arduino.eeprom.memory.BatchMemory;
import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * @author Vladimir Laskin
 * @since 22.06.2018
 */
@Getter
@AllArgsConstructor(staticName = "of")
public class BatchMemoryWrapper {
    private BatchMemory data;
    private Token token;
}
