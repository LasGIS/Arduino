/*
 *  @(#)DataXmlLoaderTest.java  last: 12.03.2023
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
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.net.URL;
import java.util.List;

import static java.util.Objects.nonNull;

/**
 * The Class DataXmlLoaderTest definition.
 *
 * @author Vladimir Laskin
 * @since 12.02.2023 : 0:40
 */
@Slf4j
class DataXmlLoaderTest {
    @Test
    void load() throws Exception {
        final ClassLoader classLoader = getClass().getClassLoader();
        final URL resource = classLoader.getResource("TestMemory.xml");
        assert resource != null;
        final File dataFile = new File(resource.toURI());
        final String targetPath = dataFile.getParentFile().getAbsolutePath();
        final MemoryRoms memoryRoms = DataXmlLoader.load(dataFile);
        LoadHelper.createDump(memoryRoms);
        Assertions.assertNotNull(memoryRoms);
        Assertions.assertEquals("test_memory", memoryRoms.getHeaderFilename());
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
                    Assertions.assertEquals(0x0000, batchMemory.getAddress());
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
        if (compareDefinitionFile(targetPath, "expected_test_memory.hex", "test_memory.hex")
            || compareDefinitionFile(targetPath, "expected_test_memory.h", "test_memory.h")) {
            Assertions.fail();
        }
    }

    private boolean compareDefinitionFile(final String targetPath, final String expectedFilename, final String actualFilename) throws Exception {
        final File expectedFile = new File(targetPath, expectedFilename);
        final File actualFile = new File(targetPath, actualFilename);
        boolean isFail = false;
        try (
            final BufferedReader expected = new BufferedReader(new FileReader(expectedFile));
            final BufferedReader actual = new BufferedReader(new FileReader(actualFile));
        ) {
            String expectedLine = expected.readLine();
            String actualLine = actual.readLine();
            int count = 0;
            log.info("{}:", actualFilename);
            while (nonNull(expectedLine) || nonNull(actualLine)) {
                count++;
//                Assertions.assertEquals(expectedLine, actualLine);
                if (!(nonNull(expectedLine) && expectedLine.equals(actualLine))) {
                    log.info("({}) Expected >{}", count, expectedLine);
                    log.info("     Actual   >{}", actualLine);
                    isFail = true;
                }
                expectedLine = expected.readLine();
                actualLine = actual.readLine();
            }
        }
        return isFail;
    }
}
