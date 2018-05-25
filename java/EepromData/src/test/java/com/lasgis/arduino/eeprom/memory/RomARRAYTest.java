/*
 *  @(#)RomARRAYTest.java  last: 25.05.2018
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
                , "0501020300FF", "[b]"
            }, {
                RomARRAY.of()
                    .add(RomINT16.of((short) 1))
                    .add(RomINT16.of((short) 2))
                    .add(RomINT16.of((short) 3))
                    .add(RomINT16.of((short) 0))
                    .add(RomINT16.of((short) -1))
                , "050001000200030000FFFF", "[i]"
            }};
        } catch (Exception ex) {
            log.error("", ex);
            throw ex;
        }
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final RomARRAY rom, final String expected, final String expectedDefine
    ) throws Exception {
        final byte[] bytes = rom.toEeprom();
        final String hexOutPrint = DatatypeConverter.printHexBinary(bytes);
        Assert.assertEquals(hexOutPrint, expected);
        Assert.assertEquals(rom.define(), expectedDefine);
    }

}