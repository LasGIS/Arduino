/*
 *  @(#)RomDOUBLETest.java  last: 19.03.2023
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
            {       -0.0, "0000000000000080"},
            {        1.0, "000000000000F03F"},
            {       -1.0, "000000000000F0BF"},
            {        0.1, "9A9999999999B93F"},
            {       -0.1, "9A9999999999B9BF"},
            {       0.01, "7B14AE47E17A843F"},
            {      -0.01, "7B14AE47E17A84BF"},
            {      100.0, "0000000000005940"},
            {     -100.0, "00000000000059C0"},
            { Double.MAX_VALUE, "FFFFFFFFFFFFEF7F"},
            {-Double.MAX_VALUE, "FFFFFFFFFFFFEFFF"},
            { Double.MIN_VALUE, "0100000000000000"},
            {-Double.MIN_VALUE, "0100000000000080"}
        };
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final double inp, final String expected
    ) throws Exception {
        final RomDOUBLE rom = RomDOUBLE.of(inp);
        final byte[] bytes = MemoryHelper.toEeprom(rom);
//        System.out.print(ByteBuffer.wrap(bytes).getDouble());
//        System.out.print(" = ");
//        System.out.println(inp);
        Assert.assertEquals(DatatypeConverter.printHexBinary(bytes), expected);
        Assert.assertEquals(rom.defChar(), 'd');
        Assert.assertEquals(rom.size(), 8);
    }

}
