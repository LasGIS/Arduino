/*
 * @(#)RomDataWrapper.java
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright © 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load.compile;

import com.lasgis.arduino.eeprom.memory.RomData;
import lombok.AllArgsConstructor;
import lombok.Getter;

/**
 * @author Vladimir Laskin
 * @since 22.06.2018
 */
@Getter
@AllArgsConstructor(staticName = "of")
public class RomDataWrapper {
    private RomData data;
    private TokenParser.Token token;
}
