/*
 *  @(#)UploadHelper.java  last: 06.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.upload;

import com.lasgis.arduino.eeprom.Runner;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.serial.PortReader;
import com.lasgis.serial.PortReaderListener;
import com.lasgis.util.ByteArrayBuilder;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.io.FilenameUtils;

import javax.xml.bind.DatatypeConverter;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.util.Hashtable;
import java.util.Map;
import java.util.Properties;

import static com.lasgis.arduino.eeprom.Runner.PROP_BAUD_RATE;
import static com.lasgis.arduino.eeprom.Runner.PROP_DATA_FILE;
import static com.lasgis.arduino.eeprom.Runner.PROP_DEVICE;
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
    private final PortReader portReader;

    public UploadHelper(final PortReader portReader) {
        this.portReader = portReader;
        this.portReader.addListener(this);
    }

    public static void upload() throws InterruptedException, IOException {
        final Properties prop = Runner.getProperties();
        final String fileName = FilenameUtils.removeExtension((new File(
            prop.getProperty(PROP_PATCH), prop.getProperty(PROP_DATA_FILE)
        )).getPath()) + ".hex";

        final String portName = prop.getProperty(PROP_PORT_NAME);
        final int baudRate = Integer.parseInt(prop.getProperty(PROP_BAUD_RATE));
        final byte device = Byte.parseByte(prop.getProperty(PROP_DEVICE), 16);

        final UploadHelper helper = new UploadHelper(PortReader.createPortReader(portName, baudRate));
        helper.uploadAll(device, fileName, helper.portReader);
        helper.portReader.stop();
    }

    public static void uploadFile(final byte device, final PortReader portReader) throws InterruptedException, IOException {
        final Properties prop = Runner.getProperties();
        final String fileName = FilenameUtils.removeExtension((new File(
            prop.getProperty(PROP_PATCH), prop.getProperty(PROP_DATA_FILE)
        )).getPath()) + ".hex";

        final UploadHelper helper = new UploadHelper(portReader);
        helper.uploadAll(device, fileName, portReader);
    }

    private void uploadAll(final byte device, final String fileName, final PortReader portReader) throws InterruptedException, IOException {
        createSerialBlocks(device, fileName);
        portWriterRun();
        Thread.sleep(10000);
    }

    private void createSerialBlocks(final byte device, final String fileName) throws IOException {
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
                final byte[] body = DatatypeConverter.parseHexBinary(line.substring(1));
                final byte size = (byte) body.length;
                final SerialBlock block = new SerialBlock();
                block.setDevice(device);
                block.setBody(body);
                block.setSize(size);
                block.setAddress(address);
                blockMap.put(address, block);
                log.info("[" + address + "]" + DatatypeConverter.printHexBinary(body));
                address += size;
            }
        }
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
    public void portReaderTrash(final byte[] data) {
//        log.info("Reader Trash: \"{}\"", string);
    }

    @Override
    public void portWriterRun() {
        try {
            for (final SerialBlock block : blockMap.values()) {
                if (block.isProcessed()) {
                    final byte[] dump = block.getWriteBytes();
                    log.info("hex[{}] = \"{}\"", block.address, DatatypeConverter.printHexBinary(dump));
                    log.info("\"{}\"", new String(dump, RomData.CHARSET));
                    portReader.writeByte(dump, dump.length);
                    block.setProcessed(false);
                    return;
                }
            }
        } catch (final InterruptedException ex) {
            log.error("portWriterRun" + ex.getMessage(), ex);
        }
    }

    public static void read() throws InterruptedException {
        final Properties prop = Runner.getProperties();
//        final String fileName = FilenameUtils.removeExtension((new File(
//            prop.getProperty(PROP_PATCH), prop.getProperty(PROP_DATA_FILE)
//        )).getPath()) + ".hex";

        final String portName = prop.getProperty(PROP_PORT_NAME);
        final int baudRate = Integer.parseInt(prop.getProperty(PROP_BAUD_RATE));

        final UploadHelper helper = new UploadHelper(PortReader.createPortReader(portName, baudRate));

        final ByteArrayBuilder bab = new ByteArrayBuilder(10);
        bab.put(':').put('B').put('R').put(0x57).putShort(0x0000).putShort(512).putChar('Л');
        final byte[] dump = bab.toByte();
        helper.portReader.writeByte(dump, dump.length);
        Thread.sleep(2000);
        helper.portReader.stop();
    }

    public static void readBlock(final PortReader portReader, final SerialBlock block) throws InterruptedException {
        final byte[] dump = block.getHead4ReadBytes();
        portReader.writeByte(dump, dump.length);
    }
}
