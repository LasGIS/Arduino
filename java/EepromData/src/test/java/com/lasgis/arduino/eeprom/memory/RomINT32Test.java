/*
 *  @(#)RomINT32Test.java  last: 29.05.2018
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
public class RomINT32Test {

    @DataProvider
    public Object[][] dataToByte() {
        return new Object[][]{
            {          0, "00000000"}, {         1, "00000001"},
            {         -1, "FFFFFFFF"}, {0xFFFFFFFF, "FFFFFFFF"},
            { 2147483647, "7FFFFFFF"}, {0x7FFFFFFF, "7FFFFFFF"},
            {-2147483648, "80000000"}, {0x80000000, "80000000"},
        };
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final int inp, final String expected
    ) throws Exception {
        final RomINT32 rom = RomINT32.of(inp);
        final byte[] bytes = rom.toEeprom();
        Assert.assertEquals(DatatypeConverter.printHexBinary(bytes), expected);
        Assert.assertEquals(rom.define(), "l");
        Assert.assertEquals(rom.size(), 4);
    }

}