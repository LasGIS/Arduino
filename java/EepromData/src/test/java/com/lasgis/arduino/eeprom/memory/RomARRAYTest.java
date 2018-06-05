/*
 *  @(#)RomARRAYTest.java  last: 30.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
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
                    .add(RomINT8.of((byte) 1))
                    .add(RomINT8.of((byte) 2))
                    .add(RomINT8.of((byte) 3))
                    .add(RomINT8.of((byte) 0))
                    .add(RomINT8.of((byte) -1))
                , "08000501020300FF", "[b]", 8
            }, {
                RomARRAY.of()
                    .add(RomINT16.of((short) 1))
                    .add(RomINT16.of((short) 2))
                    .add(RomINT16.of((short) 3))
                    .add(RomINT16.of((short) 0))
                    .add(RomINT16.of((short) -1))
                , "0D00050100020003000000FFFF", "[i]", 13
            }, {
                RomARRAY.of("ARRAY")
                    .add(RomOBJECT.of("OBJECT")
                        .add(RomINT8.of("INT8", (byte) 1))
                        .add(RomINT16.of("INT16", (short) 111))
                        .add(RomSTRING.of("STRING", "Первый объект"))
                        .add(RomCHAR.of("CHAR", 'F'))
                    )
                    .add(RomOBJECT.of("OBJECT")
                        .add(RomINT8.of((byte) 2))
                        .add(RomINT16.of((short) 222))
                        .add(RomSTRING.of("Второй объект 2"))
                        .add(RomCHAR.of('S'))
                    )
                    .add(RomOBJECT.of()
                        .add(RomINT8.of((byte) 3))
                        .add(RomINT16.of((short) 333))
                        .add(RomSTRING.of("Третий объект 3 3"))
                        .add(RomCHAR.of('T'))
                ),
                "4B0003160004016F000F00CFE5F0E2FBE920EEE1FAE5EAF246" +
                    "18000402DE001100C2F2EEF0EEE920EEE1FAE5EAF2203253" +
                    "1A0004034D011300D2F0E5F2E8E920EEE1FAE5EAF22033203354",
                "[{bisc}]", 75
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
        testCompositeRom(rom, expected, expectedDefine, size);
    }

}