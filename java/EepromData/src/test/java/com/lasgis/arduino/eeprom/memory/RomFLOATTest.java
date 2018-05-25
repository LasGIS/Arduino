/*
 *  @(#)RomFLOATTest.java  last: 25.05.2018
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
public class RomFLOATTest {

    @DataProvider
    public Object[][] dataToByte() {
        return new Object[][]{
            {(float)      0.0, "00000000"},
            {(float)     -0.0, "80000000"},
            {(float)      1.0, "3F800000"},
            {(float)     -1.0, "BF800000"},
            {(float)      0.1, "3DCCCCCD"},
            {(float)     -0.1, "BDCCCCCD"},
            {(float)     0.01, "3C23D70A"},
            {(float)    -0.01, "BC23D70A"},
            {(float)    100.0, "42C80000"},
            {(float)   -100.0, "C2C80000"},
            { Float.MAX_VALUE, "7F7FFFFF"},
            {-Float.MAX_VALUE, "FF7FFFFF"},
            { Float.MIN_VALUE, "00000001"},
            {-Float.MIN_VALUE, "80000001"}
        };
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final float inp, final String expected
    ) throws Exception {
        final RomFLOAT rom = RomFLOAT.of(inp);
        final byte[] bytes = rom.toEeprom();
//        System.out.print(ByteBuffer.wrap(bytes).getFloat());
//        System.out.print(" = ");
//        System.out.println(inp);
        Assert.assertEquals(DatatypeConverter.printHexBinary(bytes), expected);
        Assert.assertEquals(rom.define(), "f");
    }

}