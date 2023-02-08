/*
 *  @(#)RomINT32Test.java  last: 08.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
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
            {          0, "00000000"}, {         1, "01000000"},
            {         -1, "FFFFFFFF"}, {0xFFFFFFFF, "FFFFFFFF"},
            { 2147483647, "FFFFFF7F"}, {0x7FFFFFFF, "FFFFFF7F"},
            {-2147483648, "00000080"}, {0x80000000, "00000080"},
        };
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final int inp, final String expected
    ) throws Exception {
        final RomINT32 rom = RomINT32.of(inp);
        final byte[] bytes = rom.toEeprom();
        Assert.assertEquals(DatatypeConverter.printHexBinary(bytes), expected);
        Assert.assertEquals(rom.defChar(), 'l');
        Assert.assertEquals(rom.size(), 4);
    }

}
