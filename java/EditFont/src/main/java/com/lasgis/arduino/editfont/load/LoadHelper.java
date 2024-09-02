/*
 *  @(#)LoadHelper.java  last: 02.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.load;

import com.lasgis.arduino.editfont.Runner;
import com.lasgis.arduino.editfont.data.FontData;
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

    /**
     * Компиляция исходных данных
     *
     * @return FontData
     */
    public static FontData load() {
        final Properties props = Runner.getProperties();
        final File fontCFile = new File(
            props.getProperty(PROP_PATCH),
            props.getProperty(PROP_FONT_C_FILE)
        );
        final File fontHFile = new File(
            props.getProperty(PROP_PATCH),
            props.getProperty(PROP_FONT_H_FILE)
        );
        return DataCppLoader.load(fontCFile, fontHFile);
    }
}
