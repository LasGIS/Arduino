/*
 *  @(#)TestHelper.java  last: 25.08.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.test;

import com.lasgis.arduino.editfont.fontdata.FontData;
import lombok.extern.slf4j.Slf4j;

/**
 * Помощник для тестовых проверок и показов
 *
 * @author Vladimir Laskin
 * @since 31.05.2018
 */
@Slf4j
public class TestHelper {

    private static final TestHelper helper = new TestHelper();

    public static void show(final FontData fontData) {
        helper.showFontData(fontData);
    }

    private void showFontData(final FontData fontData) {
        log.info("\n FileName: \"{}\";\n", fontData.getFileName());
        final String source = fontData.getSource().toString();
        final String[] split = source.split("\n");
        log.info("\n Source: \n");
        for (final String str : split) {
            log.info(str);
        }
    }
}
