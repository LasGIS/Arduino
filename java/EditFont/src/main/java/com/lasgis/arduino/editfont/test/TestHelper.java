/*
 *  @(#)TestHelper.java  last: 22.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.test;

import com.lasgis.arduino.editfont.data.FontDataAdapter;
import com.lasgis.arduino.editfont.data.FontDataPerformed;
import lombok.extern.slf4j.Slf4j;

import java.io.File;

/**
 * Помощник для тестовых проверок и показов
 *
 * @author Vladimir Laskin
 * @since 31.05.2018
 */
@Slf4j
public class TestHelper {

    private static final TestHelper helper = new TestHelper();

    public static void show() {
        helper.showFontData();
    }

    private void showFontData() {
        FontDataPerformed.addListener(new FontDataAdapter() {
            @Override
            public void onChangeCFile(File file) {
                log.info("\n Cpp FileName: \"{}\";\n", file.getAbsolutePath());
            }

            @Override
            public void onChangeHFile(File file) {
                log.info("\n Header FileName: \"{}\";\n", file.getAbsolutePath());
            }

            @Override
            public void onChangeCSource(StringBuilder stringBuilder) {
                final String[] split = stringBuilder.toString().split("\n");
                log.info("\n Cpp Source: \n");
                for (final String str : split) {
                    log.info(str);
                }
            }

            @Override
            public void onChangeHSource(StringBuilder stringBuilder) {
                final String[] split = stringBuilder.toString().split("\n");
                log.info("\n Header Source: \n");
                for (final String str : split) {
                    log.info(str);
                }
            }
        });
    }
}
