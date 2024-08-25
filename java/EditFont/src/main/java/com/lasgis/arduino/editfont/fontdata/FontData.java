/*
 *  @(#)FontData.java  last: 25.08.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.fontdata;

import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * The Class FontData definition.
 *
 * @author Vladimir Laskin
 * @since 25.08.2024 : 17:30
 */
@Data
@NoArgsConstructor(staticName = "of")
public class FontData {
    private String fileName;
    private StringBuilder source;
}
