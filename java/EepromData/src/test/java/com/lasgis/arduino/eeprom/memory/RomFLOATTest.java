/*
 *  @(#)RomFLOATTest.java  last: 08.02.2023
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
public class RomFLOATTest {

    @DataProvider
    public Object[][] dataToByte() {
        return new Object[][]{
            {(float)      0.0, "00000000"},
            {(float)     -0.0, "00000080"},
            {(float)      1.0, "0000803F"},
            {(float)     -1.0, "000080BF"},
            {(float)      0.1, "CDCCCC3D"},
            {(float)     -0.1, "CDCCCCBD"},
            {(float)     0.01, "0AD7233C"},
            {(float)    -0.01, "0AD723BC"},
            {(float)    100.0, "0000C842"},
            {(float)   -100.0, "0000C8C2"},
            { Float.MAX_VALUE, "FFFF7F7F"},
            {-Float.MAX_VALUE, "FFFF7FFF"},
            { Float.MIN_VALUE, "01000000"},
            {-Float.MIN_VALUE, "01000080"}
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
        Assert.assertEquals(rom.defChar(), 'f');
        Assert.assertEquals(rom.size(), 4);
    }

}
