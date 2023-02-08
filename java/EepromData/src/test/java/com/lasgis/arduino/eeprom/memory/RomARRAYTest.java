/*
 *  @(#)RomARRAYTest.java  last: 09.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.extern.slf4j.Slf4j;
import org.testng.Assert;
import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

/**
 * @author Vladimir Laskin
 * @since 18.05.2018
 */
@Slf4j
public class RomARRAYTest extends RomCommonTest {

    @DataProvider
    public Object[][] dataToByte() {
        try {
            return new Object[][]{{
                RomARRAY.of()
                    .add(RomINT8.of(1))
                    .add(RomINT8.of(0xFBCDEF02))
                    .add(RomINT8.of(3))
                    .add(RomINT8.of(0))
                    .add(RomINT8.of(-1))
                , "0A0062050001020300FF", "b", 10
            }, {
                RomARRAY.of()
                    .add(RomINT16.of(1))
                    .add(RomINT16.of(0xFFBC0002))
                    .add(RomINT16.of(3))
                    .add(RomINT16.of(0))
                    .add(RomINT16.of(-3))
                , "0F006905000100020003000000FDFF", "i", 15
            }, {
                RomARRAY.of("ARRAY")
                    .add(RomOBJECT.of("OBJECT")
                        .add(RomINT8.of("INT8", 1))
                        .add(RomINT16.of("INT16", 111))
                        .add(RomSTRING.of("STRING", "Первый объект"))
                        .add(RomCHAR.of("CHAR", 'F')))
                    .add(RomOBJECT.of("OBJECT")
                        .add(RomINT8.of(2))
                        .add(RomINT16.of(222))
                        .add(RomSTRING.of("Второй объект 2"))
                        .add(RomCHAR.of('S')))
                    .add(RomOBJECT.of()
                        .add(RomINT8.of(3))
                        .add(RomINT16.of(333))
                        .add(RomSTRING.of("Третий объект 3 3"))
                        .add(RomCHAR.of('T'))
                ),
                "5C006F0300" +
                    "1B00060062697363016F000F00CFE5F0E2FBE920EEE1FAE5EAF246" +
                    "1D0006006269736302DE001100C2F2EEF0EEE920EEE1FAE5EAF2203253" +
                    "1F00060062697363034D011300D2F0E5F2E8E920EEE1FAE5EAF22033203354",
                "o", 92
            }};
        } catch (Exception ex) {
            log.error("", ex);
            Assert.fail();
            throw ex;
        }
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final RomARRAY rom, final String expected, final String expectedDefine, final int size
    ) throws Exception {
        testCompositeRom(rom, expected, 'a', expectedDefine, size);
    }

}
