/*
 *  @(#)CreateHelper.java  last: 16.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.create;

import com.lasgis.arduino.eeprom.Runner;
import com.lasgis.arduino.eeprom.memory.BatchMemory;
import com.lasgis.arduino.eeprom.memory.MemoryRoms;
import com.lasgis.arduino.eeprom.memory.RomARRAY;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.arduino.eeprom.memory.RomOBJECT;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.lang3.StringUtils;

import javax.xml.bind.DatatypeConverter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.nio.charset.Charset;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import static com.lasgis.arduino.eeprom.Runner.PROP_PATCH;

/**
 * <pre>
 * создание файлов:
 *   Memory.h       - определители (#define ...)
 *   I2CMemory.hex  - образ памяти
 * </pre>
 *
 * @author Vladimir Laskin
 * @since 31.05.2018
 */
@Slf4j
public class CreateHelper {

    private final static CreateHelper helper = new CreateHelper();

    private final static ArrayList<String> romNameList = new ArrayList<>();

    private final static int HEX_SIZE_STR_LEN = 16;

    public static void create(final MemoryRoms memoryRoms) throws IOException {
        final String headerFilename = FilenameUtils.removeExtension(memoryRoms.getHeaderFilename());
        final Properties props = Runner.getProperties();
        for (BatchMemory batchMemory : memoryRoms.getList()) {
            final byte[] dump = batchMemory.getDump();
            final String filename = Path.of(props.getProperty(PROP_PATCH), batchMemory.getPrefix() + headerFilename + ".hex").toString();
            helper.createHexDumpFile(filename, dump);
        }
        romNameList.clear();
        helper.createDefinitionFile(memoryRoms);
    }

    private void createDefinitionFile(final MemoryRoms memoryRoms) throws IOException {
        final String headerFilename = FilenameUtils.removeExtension(memoryRoms.getHeaderFilename());
        final Properties props = Runner.getProperties();
        final String fileName = FilenameUtils.removeExtension((new File(props.getProperty(PROP_PATCH), headerFilename)).getPath()) + ".h";

        log.info("Definition File = \"{}\"", fileName);
        try (final OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(fileName), Charset.forName("windows-1251"))) {
            for (BatchMemory batchMemory : memoryRoms.getList()) {
                final List<RomData> dataList = batchMemory.getRomDataList();
                for (final RomData item : dataList) {
                    writeDefinition(batchMemory.getPrefix(), item, writer);
                }
            }
        }
    }

    private void writeDefinition(final String parentName, final RomData rom, final Writer writer) throws IOException {
        final String name = rom.getName();
        final String romName = (StringUtils.isNotBlank(parentName)) ? parentName + "_" + name : name;
        final String offset = String.format("%#06x", rom.getOffset());
        if (StringUtils.isNotBlank(name)) {
            // проверка на уникальность имени
            if (romNameList.contains(romName)) {
                log.warn("Неуникальность имени: \"{}\" (адрес: {})", romName, offset);
            }
            romNameList.add(romName);

            writer.write("#define " + romName + " " + offset + "\n");
        }
        if (rom instanceof RomOBJECT) {
            for (final RomData inst : ((RomOBJECT) rom).getArray()) {
                writeDefinition(romName, inst, writer);
            }
        } else if (rom instanceof RomARRAY) {
            final List<RomData> array = ((RomARRAY) rom).getArray();
            for (int i = 0; i < array.size(); i++) {
                final RomData inst = array.get(i);
                writeDefinition(romName + "_" + i, inst, writer);
            }
        }
    }

    private void createHexDumpFile(final String fileName, final byte[] dump) throws IOException {
        log.info("Hex Dump File = \"{}\"", fileName);
        try (final OutputStreamWriter writer = new OutputStreamWriter(new FileOutputStream(fileName), Charset.forName("windows-1251"))) {
            for (int j = 0; j < dump.length; j += HEX_SIZE_STR_LEN) {
                final int len = Math.min(dump.length - j, HEX_SIZE_STR_LEN);
                final byte[] buf = new byte[len];
                System.arraycopy(dump, j, buf, 0, len);
                writer.write(":" + DatatypeConverter.printHexBinary(buf) + "\n");
            }
        }
    }
}
