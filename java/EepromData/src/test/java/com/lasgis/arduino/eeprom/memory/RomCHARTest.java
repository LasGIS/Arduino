/*
 *  @(#)RomCHARTest.java  last: 19.03.2023
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
public class RomCHARTest {

    @DataProvider
    public Object[][] dataToByte() {
        return new Object[][]{
            {'A', "41"}, {'Б', "C1"}
        };
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final char inp, final String expected
    ) throws Exception {
        final RomCHAR rom = RomCHAR.of(inp);
        final byte[] bytes = MemoryHelper.toEeprom(rom);
        Assert.assertEquals(DatatypeConverter.printHexBinary(bytes), expected);
        Assert.assertEquals(rom.defChar(), 'c');
        Assert.assertEquals(rom.size(), 1);
    }
}
