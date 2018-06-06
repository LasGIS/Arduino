/*
 *  @(#)UpLoad.java  last: 31.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.upload;

import com.lasgis.arduino.eeprom.Runner;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.arduino.serial.PortReader;
import com.lasgis.arduino.serial.PortReaderListener;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.io.FilenameUtils;

import javax.xml.bind.DatatypeConverter;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Map;
import java.util.Properties;

import static com.lasgis.arduino.eeprom.Runner.PROP_BAUD_RATE;
import static com.lasgis.arduino.eeprom.Runner.PROP_DATA_FILE;
import static com.lasgis.arduino.eeprom.Runner.PROP_PATCH;
import static com.lasgis.arduino.eeprom.Runner.PROP_PORT_NAME;

/**
 * Загружаем дамп в arduino.
 *
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Slf4j
public class UploadHelper implements PortReaderListener {

    private final Map<Short, SerialBlock> blockMap = new Hashtable<>();

    public static void upload() throws InterruptedException, IOException {
        final Properties prop = Runner.getProperties();
        final String fileName = FilenameUtils.removeExtension((new File(
            prop.getProperty(PROP_PATCH), prop.getProperty(PROP_DATA_FILE)
        )).getPath()) + ".hex";

        final String portName = prop.getProperty(PROP_PORT_NAME);
        final int baudRate = Integer.valueOf(prop.getProperty(PROP_BAUD_RATE));

        final UploadHelper helper = new UploadHelper();
        final PortReader portReader = PortReader.createPortReader(portName, baudRate);
        portReader.addListener(helper);
        helper.uploadAll(fileName, portReader);
    }

    private void uploadAll(final String fileName, final PortReader portReader) throws InterruptedException, IOException {
        createSerialBlocks(fileName);
        int count = 3;
        do {
            Thread.sleep(1000);
            upload(portReader);
        } while (checkAllUpload() && --count > 0);
        portReader.stop();
    }

    private boolean checkAllUpload() {
        for (final SerialBlock block : blockMap.values()) {
            if (block.isProcessed()) {
                return true;
            }
        }
        return false;
    }

    private void createSerialBlocks(final String fileName) throws IOException {
        log.info("Hex Dump File = \"{}\"", fileName);
        blockMap.clear();
        try (
            final BufferedReader reader = new BufferedReader(
                new InputStreamReader(
                    new FileInputStream(fileName), Charset.forName("windows-1251")
                )
            )
        ) {
            String line;
            short address = 0;
            while ((line = reader.readLine()) != null) if (line.startsWith(":")) {
                final SerialBlock block = new SerialBlock();
                final byte[] body = DatatypeConverter.parseHexBinary(line.substring(1));
                final byte size = (byte) body.length;
                block.setBody(body);
                block.setSize(size);
                block.setAddress(address);
                blockMap.put(address, block);
                log.info("[" + address + "]" + DatatypeConverter.printHexBinary(body));
                address += size;
            }
        }
    }

    private void upload(final PortReader portReader) throws InterruptedException {
        for (final SerialBlock block : blockMap.values()) {
            if (block.isProcessed()) {
                final byte[] dump = block.getBytes();
                log.info("hex[{}] = \"{}\"", block.address, DatatypeConverter.printHexBinary(dump));
//                log.info("\"{}\"", new String(dump, RomData.CHARSET));
                portReader.writeByte(dump, dump.length);
                //block.setProcessed(false);
            }
        }
        final byte[] dump = new byte[20];
        portReader.writeByte(dump, dump.length);
    }

    @Override
    public void portReaderCarriageReturn(final String str) {
        if (str.startsWith(":adr=")) {
            final short address = Short.parseShort(str.substring(5));
            final SerialBlock block = blockMap.get(address);
            if (block != null) {
                block.setProcessed(false);
            }
        }
        log.info("From Arduino: {}", str);
    }

    @Override
    public void portReaderTrash(final String string) {
//        log.info("Reader Trash: \"{}\"", string);
    }
}
