/*
 *  @(#)LoadHelper.java  last: 25.08.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.load;

import com.lasgis.arduino.editfont.Runner;
import com.lasgis.arduino.editfont.fontdata.FontData;
import lombok.extern.slf4j.Slf4j;

import java.io.File;
import java.util.Properties;

import static com.lasgis.arduino.editfont.Runner.PROP_DATA_FILE;
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
        final File dataFile = new File(
            props.getProperty(PROP_PATCH),
            props.getProperty(PROP_DATA_FILE)
        );
        final FontData fontData = DataCppLoader.load(dataFile);

        return fontData;
    }
}
