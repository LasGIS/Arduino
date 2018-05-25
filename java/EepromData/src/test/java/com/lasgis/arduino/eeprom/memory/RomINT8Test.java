/*
 *  @(#)RomINT8Test.java  last: 25.05.2018
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
public class RomINT8Test {

    @DataProvider
    public Object[][] dataToByte() {
        return new Object[][]{
            {(byte) 0, "00"}, {(byte) 1, "01"}, {(byte) -1, "FF"},  {(byte) 0xff, "FF"},
            {(byte) 127, "7F"},  {(byte) 0x7f, "7F"},
            {(byte)-128, "80"},  {(byte) 0x80, "80"},
        };
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final byte inp, final String expected
    ) throws Exception {
        final RomINT8 rom = RomINT8.of(inp);
        final byte[] bytes = rom.toEeprom();
        Assert.assertEquals(DatatypeConverter.printHexBinary(bytes), expected);
        Assert.assertEquals(rom.define(), "b");
    }

}