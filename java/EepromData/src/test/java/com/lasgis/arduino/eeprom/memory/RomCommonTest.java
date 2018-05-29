/*
 *  @(#)RomCommonTest.java  last: 29.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.extern.slf4j.Slf4j;
import org.testng.Assert;

import javax.xml.bind.DatatypeConverter;

/**
 * Общие проверки.
 *
 * @author Vladimir Laskin
 * @since 29.05.2018
 */
@Slf4j
class RomCommonTest {

    void testCompositeRom(
        final RomData rom, final String expected, final String expectedDefine, final int size
    ) throws Exception {
        final byte[] bytes = rom.toEeprom();
        final String hexOutPrint = DatatypeConverter.printHexBinary(bytes);
        log.info("\"{}\"", new String(bytes, RomData.CHARSET));
        Assert.assertEquals(hexOutPrint, expected);
        Assert.assertEquals(rom.define(), expectedDefine);
        Assert.assertEquals(rom.size(), size);
    }

}
