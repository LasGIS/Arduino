/*
 *  @(#)FontData.java  last: 22.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.data;

import lombok.Data;
import lombok.NoArgsConstructor;

import java.io.File;

/**
 * The Class FontData definition.
 *
 * @author Vladimir Laskin
 * @since 25.08.2024 : 17:30
 */
@Data
@NoArgsConstructor(staticName = "of")
public class FontData {
    private File cFile;
    private File hFile;
    private StringBuilder cSource;
    private StringBuilder hSource;
    private String fontKey;

    /** nr_chrs <Число знаков у шрифта /> */
    private Integer numberChars;
    /** chr_hgt <Высота знаков />*/
    private Integer charHeight;
    /** baseline <Средняя линия /> */
    private Integer baseLine;
    /** data_size <Число пикселей на байт /> */
    private Integer dataSize;
    /** firstchr <Первый знак /> */
    private Integer firstChar;
    /** <Ширина знака /> */
    private Integer widthChar;
}
