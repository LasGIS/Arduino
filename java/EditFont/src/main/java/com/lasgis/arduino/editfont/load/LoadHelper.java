/*
 *  @(#)LoadHelper.java  last: 06.10.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.load;

import com.lasgis.ParseException;
import com.lasgis.arduino.editfont.Runner;
import com.lasgis.arduino.editfont.data.FontDataPerformed;
import lombok.extern.slf4j.Slf4j;

import java.io.File;
import java.util.Properties;

import static com.lasgis.arduino.editfont.Runner.PROP_FONT_C_FILE;
import static com.lasgis.arduino.editfont.Runner.PROP_FONT_H_FILE;
import static com.lasgis.arduino.editfont.Runner.PROP_PATCH;

/**
 * Диспетчер по чтению входных данных.
 *
 * @author Vladimir Laskin
 * @since 31.05.2018
 */
@Slf4j
public class LoadHelper {
    /** Число знаков у шрифта */
    static final String NUMBER_CHARS_KEY = "nr_chrs";

    /** Высота знаков */
    static final String CHAR_HEIGHT_KEY = "chr_hgt";

    /** Средняя линия */
    static final String BASE_LINE_KEY = "baseline";

    /** ? Число пикселей на байт ? */
    static final String DATA_SIZE_KEY = "data_size";

    /** Первый знак */
    static final String FIRST_CHAR_KEY = "firstchr";

    /**
     * Компиляция исходных данных
     */
    public static void load() throws ParseException {
        final CppLoader cppLoader = new CppLoader();
        FontDataPerformed.addListener(cppLoader.fontDataListener);

        final HeadLoader headLoader = new HeadLoader();
        FontDataPerformed.addListener(headLoader.fontDataListener);

        final Properties props = Runner.getProperties();
        final File fontCFile = new File(
            props.getProperty(PROP_PATCH),
            props.getProperty(PROP_FONT_C_FILE)
        );
        final File fontHFile = new File(
            props.getProperty(PROP_PATCH),
            props.getProperty(PROP_FONT_H_FILE)
        );
/*
        final FontData fontData = FontDataPerformed.getFontData();
        fontData.setFontKey(props.getProperty(PROP_FONT_KEY));
*/
        FontDataPerformed.setCFile(fontCFile);
        FontDataPerformed.setHFile(fontHFile);
    }
}
