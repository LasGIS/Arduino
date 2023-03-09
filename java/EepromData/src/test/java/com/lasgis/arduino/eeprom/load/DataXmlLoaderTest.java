/*
 *  @(#)DataXmlLoaderTest.java  last: 09.03.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */
package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.memory.BatchMemory;
import com.lasgis.arduino.eeprom.memory.MemoryRoms;
import com.lasgis.arduino.eeprom.memory.RomData;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.net.URL;
import java.util.List;

/**
 * The Class DataXmlLoaderTest definition.
 *
 * @author Vladimir Laskin
 * @since 12.02.2023 : 0:40
 */
class DataXmlLoaderTest {
    @Test
    void load() throws Exception {
        final ClassLoader classLoader = getClass().getClassLoader();
        final URL resource = classLoader.getResource("TestMemory.xml");
        assert resource != null;
        final File dataFile = new File(resource.toURI());
        final MemoryRoms memoryRoms = DataXmlLoader.load(dataFile);
        LoadHelper.createDump(memoryRoms);
        Assertions.assertNotNull(memoryRoms);
        Assertions.assertEquals("test_memory", memoryRoms.getHeaderFilename());
        List<BatchMemory> list = memoryRoms.getList();
        Assertions.assertNotNull(list);
        list.forEach(batchMemory -> {
            switch (batchMemory.getPrefix()) {
                case "EEPROM": {
                    Assertions.assertEquals(0, batchMemory.getDevice());
                    Assertions.assertEquals(400, batchMemory.getAddress());
                    final byte[] dump = batchMemory.getDump();
                    final List<RomData> romDataList = batchMemory.getRomDataList();
                    Assertions.assertNotNull(romDataList);
                    Assertions.assertEquals(2, romDataList.size());
                    Assertions.assertNotNull(dump);
                    Assertions.assertEquals(8, dump.length);
                }
                break;
                case "CHIP": {
                    Assertions.assertEquals(0x57, batchMemory.getDevice());
                    Assertions.assertEquals(0, batchMemory.getAddress());
                    final byte[] dump = batchMemory.getDump();
                    final List<RomData> romDataList = batchMemory.getRomDataList();
                    Assertions.assertNotNull(romDataList);
                    Assertions.assertEquals(12, romDataList.size());
                    Assertions.assertNotNull(dump);
                    Assertions.assertEquals(485, dump.length);
                }
                break;
                default:
                    Assertions.fail();
            }
        });
        Assertions.assertEquals("test_memory", memoryRoms.getHeaderFilename());
    }
}
