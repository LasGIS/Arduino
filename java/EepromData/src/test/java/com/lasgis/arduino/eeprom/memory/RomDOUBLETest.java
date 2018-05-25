/*
 *  @(#)RomDOUBLETest.java  last: 25.05.2018
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
 * todo: add description.
 *
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
public class RomDOUBLETest {

    @DataProvider
    public Object[][] dataToByte() {
        return new Object[][]{
            {        0.0, "0000000000000000"},
            {       -0.0, "8000000000000000"},
            {        1.0, "3FF0000000000000"},
            {       -1.0, "BFF0000000000000"},
            {        0.1, "3FB999999999999A"},
            {       -0.1, "BFB999999999999A"},
            {       0.01, "3F847AE147AE147B"},
            {      -0.01, "BF847AE147AE147B"},
            {      100.0, "4059000000000000"},
            {     -100.0, "C059000000000000"},
            { Double.MAX_VALUE, "7FEFFFFFFFFFFFFF"},
            {-Double.MAX_VALUE, "FFEFFFFFFFFFFFFF"},
            { Double.MIN_VALUE, "0000000000000001"},
            {-Double.MIN_VALUE, "8000000000000001"}
        };
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final double inp, final String expected
    ) throws Exception {
        final RomDOUBLE rom = RomDOUBLE.of(inp);
        final byte[] bytes = rom.toEeprom();
//        System.out.print(ByteBuffer.wrap(bytes).getDouble());
//        System.out.print(" = ");
//        System.out.println(inp);
        Assert.assertEquals(DatatypeConverter.printHexBinary(bytes), expected);
    }

}