/*
 *  @(#)DataCppLoader.java  last: 25.08.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.load;

import com.lasgis.arduino.editfont.fontdata.FontData;
import com.lasgis.util.Util;

import java.io.File;

/**
 * Загрузчик из *.data файла. Это с-подобная структура.
 *
 * @author Vladimir Laskin
 * @since 30.05.2018
 */
class DataCppLoader {

    public DataCppLoader() {
        super();
    }

    static FontData load(final File file) {
        final DataCppLoader loader = new DataCppLoader();
        return loader.loadFile(file);
    }

    private FontData loadFile(final File file) {
        final FontData fontData = FontData.of();
        fontData.setFileName(file.getAbsolutePath());
        fontData.setSource(Util.loadStringFromFile(file));
        return fontData;
    }
}
