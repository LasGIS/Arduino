/*
 *  @(#)RomINT16Test.java  last: 29.05.2018
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
            {(short)     0, "0000"}, {(short)      1, "0001"},
            {(short)    -1, "FFFF"}, {(short) 0xFFFF, "FFFF"},
            {(short) 32767, "7FFF"}, {(short) 0x7FFF, "7FFF"},
            {(short)-32768, "8000"}, {(short) 0x8000, "8000"},
        };
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final short inp, final String expected
    ) throws Exception {
        final RomINT16 rom = RomINT16.of(inp);
        final byte[] bytes = rom.toEeprom();
        Assert.assertEquals(DatatypeConverter.printHexBinary(bytes), expected);
        Assert.assertEquals(rom.define(), "i");
        Assert.assertEquals(rom.size(), 2);
    }

}