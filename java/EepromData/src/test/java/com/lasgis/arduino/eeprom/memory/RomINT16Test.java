/*
 *  @(#)RomINT16Test.java  last: 17.05.2018
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
 * @since 17.05.2018
 */
public class RomINT16Test {

    @DataProvider
    public Object[][] dataToByte() {
        return new Object[][]{
            {(short)     0, "0000"}, {(short)      1, "0100"},
            {(short)    -1, "FFFF"}, {(short) 0xFFFF, "FFFF"},
            {(short) 32767, "FF7F"}, {(short) 0x7FFF, "FF7F"},
            {(short)-32768, "0080"}, {(short) 0x8000, "0080"},
        };
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final short inp, final String expected
    ) throws Exception {
        final RomINT16 rom = RomINT16.of(inp);
        final byte[] bytes = rom.toByte();
        Assert.assertEquals(DatatypeConverter.printHexBinary(bytes), expected);
    }

}