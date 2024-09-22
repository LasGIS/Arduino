/*
 *  @(#)FontDataEvent.java  last: 22.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.data;

import lombok.AllArgsConstructor;
import lombok.Data;

/**
 * The Class FontDataChangeEvent definition.
 *
 * @author Vladimir Laskin
 * @since 21.09.2024 : 12:37
 */
@Data
@AllArgsConstructor(staticName = "of")
public class FontDataEvent {
    private FontDataEventKey key;
    private Object value;
}
