/*
 *  @(#)RomARRAYTest.java  last: 18.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import org.testng.Assert;
import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import javax.xml.bind.DatatypeConverter;

/**
 * @author Vladimir Laskin
 * @since 18.05.2018
 */
public class RomARRAYTest {

    @DataProvider
    public Object[][] dataToByte() {
        return new Object[][] {{
            RomARRAY.of()
                .add(RomINT8.of((byte) 1))
                .add(RomINT8.of((byte) 2))
                .add(RomINT8.of((byte) 3))
                .add(RomINT8.of((byte) 0))
                .add(RomINT8.of((byte)-1))
            , "0501020300FF"
        }, {
            RomARRAY.of()
                .add(RomINT16.of((short) 1))
                .add(RomINT16.of((short) 2))
                .add(RomINT16.of((short) 3))
                .add(RomINT16.of((short) 0))
                .add(RomINT16.of((short)-1))
            , "050100020003000000FFFF"
        } };
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final RomARRAY rom, final String expected
    ) throws Exception {
        final byte[] bytes = rom.toByte();
        final String hexOutPrint = DatatypeConverter.printHexBinary(bytes);
        Assert.assertEquals(hexOutPrint, expected);
    }

}