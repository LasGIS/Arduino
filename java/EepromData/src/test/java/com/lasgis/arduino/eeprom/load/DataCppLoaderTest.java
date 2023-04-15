/*
 *  @(#)DataCppLoaderTest.java  last: 14.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.create.CreateHelper;
import com.lasgis.arduino.eeprom.memory.BatchMemory;
import com.lasgis.arduino.eeprom.memory.MemoryRoms;
import com.lasgis.arduino.eeprom.memory.RomARRAY;
import com.lasgis.arduino.eeprom.memory.RomCHAR;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.arduino.eeprom.memory.RomFLOAT;
import com.lasgis.arduino.eeprom.memory.RomINT16;
import com.lasgis.arduino.eeprom.memory.RomINT32;
import com.lasgis.arduino.eeprom.memory.RomINT8;
import com.lasgis.arduino.eeprom.memory.RomOBJECT;
import com.lasgis.arduino.eeprom.memory.RomSTRING;
import lombok.extern.slf4j.Slf4j;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import javax.xml.bind.DatatypeConverter;
import java.io.File;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Stream;

/**
 * @author Vladimir Laskin
 * @since 15.06.2018
 */
@Slf4j
public class DataCppLoaderTest extends CompareHelper {

    private static Stream<Arguments> provideTestData() {
        return Stream.of(Arguments.of(
            "  /* Комментарий */\n"
                + "  char CHAR_SINGN_WIN : 'B'; /* Комментарий */ \n"
                + "  int8 : 255; /* Комментарий */ \n"
                + "  int16 : 65535; /* Комментарий */ \n"
                + "  int16 : -32767; /* Комментарий */ \n"
                + "  int32 : 255; /* Комментарий */ \n"
                + "  float : 3.1415926; /* Комментарий */ \n"
                + "  float : -3.1415926; /* Комментарий */ \n"
                + "  string WIN_NAME : \"Название машинки\"; /* Комментарий */ \n",
            new ArrayList<RomData>() {{
                this.add(RomCHAR.of("CHAR_SINGN_WIN", null, 'B'));
                this.add(RomINT8.of(255));
                this.add(RomINT16.of(65535));
                this.add(RomINT16.of(-32767));
                this.add(RomINT32.of(255));
                this.add(RomFLOAT.of(3.1415926));
                this.add(RomFLOAT.of(-3.1415926));
                this.add(RomSTRING.of("WIN_NAME", null, "Название машинки"));
            }}
        ), Arguments.of(
            "  /* OBJECT */\n"
                + "  char CHAR_SINGN_WIN : 'B';\n"
                + "  int8 : 255;\n"
                + "  int16 : 65535;\n"
                + "  int16 : -32767;\n"
                + "  int32 : 4294967295;\n"
                + "  int32 : 4294967296;\n"
                + "  float : 3.1415926;\n"
                + "  string WIN_NAME : \"Название машинки\";\n"
                + "  OBJECT_VEHICLE : {\n"
                + "     int8 : 255,\n"
                + "     float : 60.78,\n"
                + "     string : \"Быстрый мустанг\"\n"
                + "  };\n",
            new ArrayList<RomData>() {{
                this.add(RomCHAR.of("CHAR_SINGN_WIN", null, 'B'));
                this.add(RomINT8.of(255));
                this.add(RomINT16.of(65535));
                this.add(RomINT16.of(-32767));
                this.add(RomINT32.of(-1));
                this.add(RomINT32.of(0));
                this.add(RomFLOAT.of(3.1415926));
                this.add(RomSTRING.of("WIN_NAME", null, "Название машинки"));
                this.add(RomOBJECT.of("OBJECT_VEHICLE", null)
                    .add(RomINT8.of(255))
                    .add(RomFLOAT.of(60.78))
                    .add(RomSTRING.of("Быстрый мустанг"))
                );
            }}
        ), Arguments.of(
            "  /* ARRAY */\n"
                + "  char CHAR_SINGN_WIN : 'B';\n"
                + "  ARRAY_of_CHAR : [\n"
                + "     char : 'A',\n"
                + "     char : 'a',\n"
                + "     char : 'А',\n"
                + "     char : 'а'\n"
                + "  ];\n"
                + "  ARRAY_of_INT16 : [\n"
                + "     int16 : 31415,\n"
                + "     int16 : 6078,\n"
                + "     int16 : -31415\n"
                + "  ];\n"
                + "  ARRAY_of_DOUBLE : [\n"
                + "     float : 3.1415926,\n"
                + "     float : 60.78,\n"
                + "     float : -3.1415926\n"
                + "  ];\n",
            new ArrayList<RomData>() {{
                this.add(RomCHAR.of("CHAR_SINGN_WIN", null, 'B'));
                this.add(RomARRAY.of("ARRAY_of_CHAR", null)
                    .add(RomCHAR.of('A'))
                    .add(RomCHAR.of('a'))
                    .add(RomCHAR.of('А'))
                    .add(RomCHAR.of('а')));
                this.add(RomARRAY.of("ARRAY_of_INT16", null)
                    .add(RomINT16.of(31415))
                    .add(RomINT16.of(6078))
                    .add(RomINT16.of(-31415)));
                this.add(RomARRAY.of("ARRAY_of_DOUBLE", null)
                    .add(RomFLOAT.of(3.1415926))
                    .add(RomFLOAT.of(60.78))
                    .add(RomFLOAT.of(-3.1415926)));
            }}
        ), Arguments.of(
            "  /* ARRAY Define type */\n"
                + "  char CHAR_SINGN_WIN : 'B';\n"
                + "  ARRAY_of_CHAR char : ['A', 'a','А','а'];\n"
                + "  ARRAY_of_INT16 int16 : [31415, 6078, -31415];\n"
                + "  ARRAY_of_DOUBLE float : [ Число_Пи : 3.1415926, 60.78, -3.1415926];\n",
            new ArrayList<RomData>() {{
                this.add(RomCHAR.of("CHAR_SINGN_WIN", null, 'B'));
                this.add(RomARRAY.of("ARRAY_of_CHAR", null)
                    .add(RomCHAR.of('A'))
                    .add(RomCHAR.of('a'))
                    .add(RomCHAR.of('А'))
                    .add(RomCHAR.of('а')));
                this.add(RomARRAY.of("ARRAY_of_INT16", null)
                    .add(RomINT16.of(31415))
                    .add(RomINT16.of(6078))
                    .add(RomINT16.of(-31415)));
                this.add(RomARRAY.of("ARRAY_of_DOUBLE", null)
                    .add(RomFLOAT.of("Число_Пи", null, 3.1415926))
                    .add(RomFLOAT.of(60.78))
                    .add(RomFLOAT.of(-3.1415926)));
            }}
        ));
    }

    @ParameterizedTest
    @MethodSource("provideTestData")
    public void testLoad(
        final String data,
        final ArrayList<RomData> expectedList
    ) throws Exception {
        final DataCppLoader loader = new DataCppLoader();
        loader.setProgramCode(new StringBuilder(data));
        final MemoryRoms memoryRoms = loader.parse();
        final BatchMemory batchMemory = memoryRoms.getList().get(0);
        Assertions.assertEquals(batchMemory.getRomDataList().size(), expectedList.size(), "Размер списка разный");
        final List<RomData> list = batchMemory.getRomDataList();
        for (int i = 0; i < expectedList.size(); i++) {
            final RomData expectedData = expectedList.get(i);
            final RomData loaderData = list.get(i);
            Assertions.assertEquals(loaderData, expectedData);
        }
    }

    @Test
    void loadRomMemory() throws Exception {
        final ClassLoader classLoader = getClass().getClassLoader();
        final URL resource = classLoader.getResource("RomMemory.data");
        assert resource != null;
        final File dataFile = new File(resource.toURI());
        final String targetPath = dataFile.getParentFile().getAbsolutePath();
        final MemoryRoms memoryRoms = DataCppLoader.load(dataFile);
        Assertions.assertNotNull(memoryRoms);
        LoadHelper.createDump(memoryRoms);
        List<BatchMemory> list = memoryRoms.getList();
        Assertions.assertNotNull(list);
        list.forEach(batchMemory -> {
            final byte[] dump = batchMemory.getDump();
            final List<RomData> romDataList = batchMemory.getRomDataList();
            Assertions.assertNotNull(romDataList);
            Assertions.assertEquals(15, romDataList.size());
            Assertions.assertNotNull(dump);
            log.info("dump: {}", DatatypeConverter.printHexBinary(dump));
            Assertions.assertEquals(139, dump.length);
        });
        Assertions.assertEquals("rom_memory", memoryRoms.getHeaderFilename());
        CreateHelper.create(targetPath, memoryRoms);
        boolean isHexFail = compareDefinitionFile(targetPath, "expected_rom_memory.hex", "rom_memory.hex");
        boolean isDefFail = compareDefinitionFile(targetPath, "expected_rom_memory.h", "rom_memory.h");
        if (isHexFail || isDefFail) {
            Assertions.fail();
        }
    }

    @Test
    void loadTestMemory() throws Exception {
        final ClassLoader classLoader = getClass().getClassLoader();
        final URL resource = classLoader.getResource("TestMemory.data");
        assert resource != null;
        final File dataFile = new File(resource.toURI());
        final String targetPath = dataFile.getParentFile().getAbsolutePath();
        final MemoryRoms memoryRoms = DataCppLoader.load(dataFile);
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
                    Assertions.assertEquals(0x0700, batchMemory.getAddress());
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
