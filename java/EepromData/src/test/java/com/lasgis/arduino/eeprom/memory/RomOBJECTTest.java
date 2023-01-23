/*
 *  @(#)RomOBJECTTest.java  last: 24.01.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.extern.slf4j.Slf4j;
import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

/**
 * @author Vladimir Laskin
 * @since 18.05.2018
 */
@Slf4j
public class RomOBJECTTest extends RomCommonTest {

    @DataProvider
    public Object[][] dataToByte() {
        return new Object[][] {{
            RomOBJECT.of()
                .add(RomINT8.of((byte) 1))
                .add(RomINT16.of((short) 2))
                .add(RomINT32.of(3))
                .add(RomINT8.of((byte) 0))
                .add(RomINT8.of((byte)-1))
            , "140009007B62696C62627D0102000300000000FF", "{bilbb}", 20
        }, {
            RomOBJECT.of()
                .add(RomINT8.of((byte) 1))
                .add(RomINT16.of((short) 2))
                .add(RomSTRING.of("Text in English"))
                .add(RomSTRING.of("Текст на Русском"))
            , "300008007B626973737D01020011005465787420696E20456E676C6973681200D2E5EAF1F220EDE020D0F3F1F1EAEEEC"
            , "{biss}", 48
        } };
    }
    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final RomOBJECT rom, final String expected, final String expectedDefine, final int size
    ) throws Exception {
        testCompositeRom(rom, expected, expectedDefine, size);
    }
}
