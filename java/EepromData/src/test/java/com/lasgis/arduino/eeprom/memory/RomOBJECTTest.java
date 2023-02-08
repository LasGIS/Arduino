/*
 *  @(#)RomOBJECTTest.java  last: 08.02.2023
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
            , "1200070062696C62620102000300000000FF", "bilbb", 18
        }, {
            RomOBJECT.of()
                .add(RomINT8.of((byte) 1))
                .add(RomINT16.of((short) 2))
                .add(RomSTRING.of("Text in English"))
                .add(RomSTRING.of("Текст на Русском"))
            , "2E0006006269737301020011005465787420696E20456E676C6973681200D2E5EAF1F220EDE020D0F3F1F1EAEEEC"
            , "biss", 46
        } };
    }
    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final RomOBJECT rom, final String expected, final String expectedDefine, final int size
    ) throws Exception {
        testCompositeRom(rom, expected, 'o', expectedDefine, size);
    }
}
