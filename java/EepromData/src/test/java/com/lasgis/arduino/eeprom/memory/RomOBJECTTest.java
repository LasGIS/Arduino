/*
 *  @(#)RomOBJECTTest.java  last: 25.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.extern.slf4j.Slf4j;
import org.testng.Assert;
import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import javax.xml.bind.DatatypeConverter;

/**
 * @author Vladimir Laskin
 * @since 18.05.2018
 */
@Slf4j
public class RomOBJECTTest {

    @DataProvider
    public Object[][] dataToByte() {
        return new Object[][] {{
            RomOBJECT.of()
                .add(RomINT8.of((byte) 1))
                .add(RomINT16.of((short) 2))
                .add(RomINT32.of(3))
                .add(RomINT8.of((byte) 0))
                .add(RomINT8.of((byte)-1))
            , "050100020000000300FF", "{bilbb}"
        }, {
            RomOBJECT.of()
                .add(RomINT8.of((byte) 1))
                .add(RomINT16.of((short) 2))
                .add(RomSTRING.of("Text in English"))
                .add(RomSTRING.of("Текст на Русском"))
            , "040100020F5465787420696E20456E676C69736810D2E5EAF1F220EDE020D0F3F1F1EAEEEC"
            , "{biss}"
        } };
    }
    @Test(dataProvider = "dataToByte")
    public void testToByte(
        final RomOBJECT rom, final String expected, final String expectedDefine
    ) throws Exception {
        final byte[] bytes = rom.toEeprom();
        final String hexOutPrint = DatatypeConverter.printHexBinary(bytes);
        log.info("\"{}\"", new String(bytes, RomData.CHARSET));
        Assert.assertEquals(hexOutPrint, expected);
        Assert.assertEquals(rom.define(), expectedDefine);
    }

}