/*
 *  @(#)RomSTRINGTest.java  last: 25.05.2018
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
 * @since 16.05.2018
 */
public class RomSTRINGTest {

    @DataProvider
    public Object[][] dataToByte() {
        return new Object[][]{
            {"ABCD", "0441424344"},
            {"abcd", "0461626364"},
            {"АБВГД", "05C0C1C2C3C4"},
            {"абвгд", "05E0E1E2E3E4"},
            { " !\"#$%&'()*+,-./0123456789:;<=>?"
            + "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
            + "`abcdefghijklmnopqrstuvwxyz{|}~",
            "5F202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F"
            + "404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F"
            + "606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E"
            }, {
              "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯЁ"
            + "абвгдежзийклмнопрстуфхцчшщъыьэюяё",
            "42C0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDFA8"
            + "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFFB8"
            }, {
              " !\"#$%&'()*+,-./0123456789:;<=>?"
            + "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_"
            + "`abcdefghijklmnopqrstuvwxyz{|}~"
            + "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
            + "абвгдежзийклмнопрстуфхцчшщъыьэюя",
            "9F202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F"
            + "404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F"
            + "606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E"
            + "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
            + "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF"
            }
        };
    }

    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final String inp, final String hexOut
    ) throws Exception {
        final RomSTRING rom = RomSTRING.of(inp);
        final byte[] bytes = rom.toEeprom();
        final String hexOutPrint = DatatypeConverter.printHexBinary(bytes);
//        System.out.println("inp = \"" + inp + "\"");
//        System.out.println("out = \"" + hexOutPrint + "\"");
        Assert.assertEquals(hexOutPrint, hexOut);
    }

}