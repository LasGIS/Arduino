/*
 *  @(#)UploadHelperTest.java  last: 17.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.upload;

import org.apache.commons.lang3.StringUtils;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

import static com.lasgis.util.Util.parseHexByte;
import static com.lasgis.util.Util.parseHexShort;

/**
 * The Class UploadHelperTest definition.
 *
 * @author Vladimir Laskin
 * @since 17.02.2023 : 17:39
 */
class UploadHelperTest {
    private static Stream<Arguments> paramUploadAddressStringParser() {
        return Stream.of(
            Arguments.of("prefix:EEPROM,device:0x00,offset:0x0200", "EEPROM", (byte) 0x00, (short) 0x0200),
            Arguments.of("prefix:AT24C,device:0x57,offset:0x0700", "AT24C", (byte) 87 , (short) 1792)
        );
    }

    @ParameterizedTest()
    @MethodSource("paramUploadAddressStringParser")
    public void testUploadAddressStringParser(final String input, final String expectedPrefix, final byte expectedDevice, final short expectedOffset) {
        final String[] spl = StringUtils.split(input, ',');
        String prefix = "PREFIX";
        byte device = (byte) 0xff;
        short offset = (short) 0xffff;
        for (final String param : spl) {
            final String[] splitParam = StringUtils.split(param, ':');
            switch (splitParam[0]) {
                case "prefix":
                    prefix = splitParam[1];
                    break;
                case "device":
                    device = parseHexByte(splitParam[1]);
                    break;
                case "offset":
                    offset = parseHexShort(splitParam[1]);
                    break;
            }
        }
        Assertions.assertEquals(expectedPrefix, prefix);
        Assertions.assertEquals(expectedDevice, device);
        Assertions.assertEquals(expectedOffset, offset);
    }

}
