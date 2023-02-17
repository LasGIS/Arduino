/*
 *  @(#)BatchDump.java  last: 17.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.Data;
import lombok.RequiredArgsConstructor;

/**
 * The Class BatchDump содержит дамп загрузки в ROM (ПЗУ).
 *
 * @author Vladimir Laskin
 * @since 15.02.2023 : 14:15
 */
@Data
@RequiredArgsConstructor(staticName = "of")
public class BatchDump {
    private final byte[] dump;
    private final byte device;
    private final int address;
}
