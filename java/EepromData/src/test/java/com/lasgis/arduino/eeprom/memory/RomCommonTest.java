/*
 *  @(#)RomCommonTest.java  last: 19.03.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.extern.slf4j.Slf4j;
import org.apache.commons.lang3.StringUtils;
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
    private final static String TAB = " ";

    void testCompositeRom(
        final RomData rom, final String expected, final Character expectedDefChar, final String expectedDefine, final int size
    ) throws Exception {
        final byte[] bytes = MemoryHelper.toEeprom(rom);
        final String hexOutPrint = DatatypeConverter.printHexBinary(bytes);
        log.info("\"{}\"", new String(bytes, RomData.CHARSET));
        Assert.assertEquals(hexOutPrint, expected);
        Assert.assertEquals(rom.defChar(), expectedDefChar);
        Assert.assertEquals(rom.define(), expectedDefine);
        Assert.assertEquals(rom.size(), size);
        showNameOffset(rom);
    }

    private void showNameOffset(final RomData rom) {
        final StringBuilder sb = new StringBuilder("\n");
        showNameOffset(rom, sb, "");
        log.info(sb.toString());
    }

    private void showNameOffset(final RomData rom, final StringBuilder sb, final String tab) {
        final String name = rom.getName();
        if (StringUtils.isNotBlank(name)) {
            sb.append(tab).append("name: ").append(rom.getName()).append("; ");
        } else {
            sb.append(tab);
        }
        sb.append("offset: ").append(rom.getOffset()).append(";");
        if (rom instanceof RomOBJECT) {
            sb.append(" {\n");
            for (final RomData inst : ((RomOBJECT) rom).getArray()) {
                showNameOffset(inst, sb, tab + TAB);
            }
            sb.append(tab).append("}\n");
        } else if (rom instanceof RomARRAY) {
            sb.append(" [\n");
            for (final RomData inst : ((RomARRAY) rom).getArray()) {
                showNameOffset(inst, sb, tab + TAB);
            }
            sb.append(tab).append("]\n");
        } else {
            sb.append("\n");
        }
    }
}
