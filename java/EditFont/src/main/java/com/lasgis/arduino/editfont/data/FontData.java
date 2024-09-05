/*
 *  @(#)FontData.java  last: 05.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.data;

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
    private String cFileName;
    private String hFileName;
    private StringBuilder cSource;
    private StringBuilder hSource;
    private String fontKey;

    /** nr_chrs */
    private Integer numberChars;
    /** chr_hgt */
    private Integer charHeight;
    /** baseline */
    private Integer baseLine;
    /** data_size */
    private Integer dataSize;
    /** firstchr */
    private Integer firstChar;

}
