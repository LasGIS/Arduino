/*
 *  @(#)TestHelper.java  last: 16.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.test;

import com.lasgis.arduino.eeprom.memory.BatchMemory;
import com.lasgis.arduino.eeprom.memory.MemoryRoms;
import com.lasgis.arduino.eeprom.memory.RomARRAY;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.arduino.eeprom.memory.RomOBJECT;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.lang3.StringUtils;

import javax.xml.bind.DatatypeConverter;
import java.util.List;

/**
 * Помощник для тестовых проверок и показов
 *
 * @author Vladimir Laskin
 * @since 31.05.2018
 */
@Slf4j
public class TestHelper {

    private static final String TAB = "  ";

    private static final TestHelper helper = new TestHelper();

    public static void show(final MemoryRoms memoryRoms) {
        for (final BatchMemory batchMemory : memoryRoms.getList()) {
            helper.showDataList(batchMemory);
        }
    }

    private void showDataList(final BatchMemory batchMemory) {
        final List<RomData> array = batchMemory.getRomDataList();
        final byte[] dump = batchMemory.getDump();
        log.info("\n byte[] = \"" + DatatypeConverter.printHexBinary(dump) + "\"");
        for (final RomData item : array) {
            showNameOffset(item);
        }
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
