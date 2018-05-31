/*
 *  @(#)CreateHelper.java  last: 31.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.create;

import com.lasgis.arduino.eeprom.Runner;
import com.lasgis.arduino.eeprom.memory.RomData;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.io.FilenameUtils;

import javax.xml.bind.DatatypeConverter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.nio.charset.Charset;
import java.util.List;
import java.util.Properties;

import static com.lasgis.arduino.eeprom.Runner.PROP_DATA_FILE;
import static com.lasgis.arduino.eeprom.Runner.PROP_PATCH;

/**
 * создание файлов:
 *   I2CMemory.h    - определители (#define ...)
 *   I2CMemory.hex  - образ памяти
 *
 * @author Vladimir Laskin
 * @since 31.05.2018
 */
@Slf4j
public class CreateHelper {

    private final static CreateHelper helper = new CreateHelper();

    private final static int HEX_SIZE_STR_LEN = 21;

    public static void create() throws IOException {
        final List<RomData> dataList = Runner.getDataList();
        final byte[] dump = Runner.getDump();
        final Properties props = Runner.getProperties();
        final String patch = FilenameUtils.removeExtension((new File(
            props.getProperty(PROP_PATCH), props.getProperty(PROP_DATA_FILE)
        )).getPath());

        helper.createDefinitionFile(patch + ".h", dataList);
        helper.createHexDumpFile(patch + ".hex", dump);
    }

    private void createDefinitionFile(final String fileName, final List<RomData> dataList) {
        log.info("Definition File = \"{}\"", fileName);

    }

    private void createHexDumpFile(final String fileName, final byte[] dump) throws IOException {
        log.info("Hex Dump File = \"{}\"", fileName);
        log.info(":\"{}\"", DatatypeConverter.printHexBinary(dump));
        final OutputStreamWriter writer = new OutputStreamWriter(
            new FileOutputStream(fileName), Charset.forName("windows-1251")
        );

        for (int j = 0; j < dump.length; j += HEX_SIZE_STR_LEN) {
            final int len = dump.length - j < HEX_SIZE_STR_LEN ? dump.length - j : HEX_SIZE_STR_LEN;
            final byte[] buf = new byte[len];
            System.arraycopy(dump, j, buf, 0, len);
            writer.write(":" + DatatypeConverter.printHexBinary(buf) + "\n");
        }
        writer.close();
    }
}
