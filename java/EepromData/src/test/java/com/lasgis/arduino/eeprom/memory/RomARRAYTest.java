/*
 *  @(#)RomARRAYTest.java  last: 29.05.2018
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

import javax.xml.bind.DatatypeConverter;

/**
 * @author Vladimir Laskin
 * @since 18.05.2018
 */
@Slf4j
public class RomARRAYTest {

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
                , "00080501020300FF", "[b]", 8
            }, {
                RomARRAY.of()
                    .add(RomINT16.of((short) 1))
                    .add(RomINT16.of((short) 2))
                    .add(RomINT16.of((short) 3))
                    .add(RomINT16.of((short) 0))
                    .add(RomINT16.of((short) -1))
                , "000D050001000200030000FFFF", "[i]", 13
            }, {
                RomARRAY.of()
                    .add(RomOBJECT.of()
                        .add(RomINT8.of((byte) 1))
                        .add(RomINT16.of((short) 111))
                        .add(RomSTRING.of("Первый объект"))
                        .add(RomCHAR.of('F'))
                    )
                    .add(RomOBJECT.of()
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
                "004B0300160401006F000FCFE5F0E2FBE920EEE1FAE5EAF246" +
                      "0018040200DE0011C2F2EEF0EEE920EEE1FAE5EAF2203253" +
                      "001A0403014D0013D2F0E5F2E8E920EEE1FAE5EAF22033203354",
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
        final byte[] bytes = rom.toEeprom();
        final String hexOutPrint = DatatypeConverter.printHexBinary(bytes);
        log.info("\"{}\"", new String(bytes, RomData.CHARSET));
        Assert.assertEquals(hexOutPrint, expected);
        Assert.assertEquals(rom.define(), expectedDefine);
        Assert.assertEquals(rom.size(), size);
    }

}