/*
 *  @(#)TestHelper.java  last: 24.09.2024
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
            public boolean onChangeCFile(File file) {
                log.info("\n Cpp FileName: \"{}\";\n", file.getAbsolutePath());
                return true;
            }

            @Override
            public boolean onChangeHFile(File file) {
                log.info("\n Header FileName: \"{}\";\n", file.getAbsolutePath());
                return true;
            }

            @Override
            public boolean onChangeCSource(StringBuilder stringBuilder) {
                log.info("""

                    -----------------------
                       Cpp Source:
                    -----------------------
                    {}""", stringBuilder);
                return true;
            }

            @Override
            public boolean onChangeHSource(StringBuilder stringBuilder) {
                log.info("""
                                        
                    -----------------------
                        Header Source:
                    -----------------------
                    {}""", stringBuilder);
                return true;
            }

            @Override
            public boolean onChangeWidthChar(Integer value) {
                log.info("widthChar: {}", value);
                return true;
            }

            @Override
            public boolean onChangeNumberChars(Integer value) {
                log.info("numberChars: {}", value);
                return true;
            }

            @Override
            public boolean onChangeCharHeight(Integer value) {
                log.info("charHeight: {}", value);
                return true;
            }

            @Override
            public boolean onChangeBaseLine(Integer value) {
                log.info("baseLine: {}", value);
                return true;
            }

            @Override
            public boolean onChangeDataSize(Integer value) {
                log.info("dataSize: {}", value);
                return true;
            }

            @Override
            public boolean onChangeFirstChar(Integer value) {
                log.info("firstChar: {}", value);
                return true;
            }
        });
    }
}
