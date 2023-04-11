/*
 *  @(#)DataXmlLoaderTest.java  last: 11.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */
package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.create.CreateHelper;
import com.lasgis.arduino.eeprom.memory.BatchMemory;
import com.lasgis.arduino.eeprom.memory.MemoryRoms;
import com.lasgis.arduino.eeprom.memory.RomData;
import lombok.extern.slf4j.Slf4j;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import javax.xml.bind.DatatypeConverter;
import java.io.File;
import java.net.URL;
import java.util.List;

/**
 * The Class DataXmlLoaderTest definition.
 *
 * @author Vladimir Laskin
 * @since 12.02.2023 : 0:40
 */
@Slf4j
class DataXmlLoaderTest extends CompareHelper {
    @Test
    void load() throws Exception {
        final ClassLoader classLoader = getClass().getClassLoader();
        final URL resource = classLoader.getResource("TestMemory.xml");
        assert resource != null;
        final File dataFile = new File(resource.toURI());
        final String targetPath = dataFile.getParentFile().getAbsolutePath();
        final MemoryRoms memoryRoms = DataXmlLoader.load(dataFile);
        Assertions.assertNotNull(memoryRoms);
        LoadHelper.createDump(memoryRoms);
        List<BatchMemory> list = memoryRoms.getList();
        Assertions.assertNotNull(list);
        list.forEach(batchMemory -> {
            switch (batchMemory.getPrefix()) {
                case "EEPROM": {
                    Assertions.assertEquals(0x00, batchMemory.getDevice());
                    Assertions.assertEquals(0x0400, batchMemory.getAddress());
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
                    Assertions.assertEquals(0x0400, batchMemory.getAddress());
                    final byte[] dump = batchMemory.getDump();
                    final List<RomData> romDataList = batchMemory.getRomDataList();
                    Assertions.assertNotNull(romDataList);
                    Assertions.assertEquals(13, romDataList.size());
                    Assertions.assertNotNull(dump);
                    log.info("dump: {}", DatatypeConverter.printHexBinary(dump));
                    Assertions.assertEquals(490, dump.length);
                }
                break;
                default:
                    Assertions.fail();
            }
        });
        Assertions.assertEquals("test_memory", memoryRoms.getHeaderFilename());
        CreateHelper.create(targetPath, memoryRoms);
        boolean isHexFail = compareDefinitionFile(targetPath, "expected_test_memory.hex", "test_memory.hex");
        boolean isDefFail = compareDefinitionFile(targetPath, "expected_test_memory.h", "test_memory.h");
        if (isHexFail || isDefFail) {
            Assertions.fail();
        }
    }
}
