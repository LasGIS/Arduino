/*
 *  @(#)DataCppLoader.java  last: 02.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.load;

import com.lasgis.arduino.editfont.data.FontData;
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

    static FontData load(final File cFile, final File hFile) {
        final DataCppLoader loader = new DataCppLoader();
        return loader.loadFile(cFile, hFile);
    }

    private FontData loadFile(final File cFile, final File hFile) {
        final FontData fontData = FontData.of();
        fontData.setCFileName(cFile.getName());
        fontData.setCSource(Util.loadStringFromFile(cFile));
        fontData.setHFileName(hFile.getName());
        fontData.setHSource(Util.loadStringFromFile(hFile));
        return fontData;
    }
}
