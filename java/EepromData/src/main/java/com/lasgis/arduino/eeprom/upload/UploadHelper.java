/*
 *  @(#)UploadHelper.java  last: 17.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.upload;

import com.lasgis.arduino.eeprom.Runner;
import com.lasgis.arduino.eeprom.memory.MemoryRoms;
import com.lasgis.serial.PortReader;
import com.lasgis.serial.PortReaderListener;
import com.lasgis.util.ByteArrayBuilder;
import lombok.AllArgsConstructor;
import lombok.EqualsAndHashCode;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.lang3.StringUtils;

import javax.xml.bind.DatatypeConverter;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.Charset;
import java.nio.file.Path;
import java.util.Hashtable;
import java.util.Map;
import java.util.Properties;

import static com.lasgis.arduino.eeprom.Runner.PROP_BAUD_RATE;
import static com.lasgis.arduino.eeprom.Runner.PROP_DATA_FILE;
import static com.lasgis.arduino.eeprom.Runner.PROP_PATCH;
import static com.lasgis.arduino.eeprom.Runner.PROP_PORT_NAME;
import static com.lasgis.util.Util.parseHexByte;
import static com.lasgis.util.Util.parseHexInt;
import static com.lasgis.util.Util.parseHexShort;
import static java.lang.Byte.parseByte;

/**
 * Загружаем дамп в arduino.
 *
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Slf4j
public class UploadHelper implements PortReaderListener {

    private final Map<DeviceAddress, SerialBlock> blockMap = new Hashtable<>();
    private final PortReader portReader;

    @AllArgsConstructor(staticName = "of")
    @EqualsAndHashCode
    static private class DeviceAddress {
        byte device;
        short address;
    }

    public UploadHelper(final PortReader portReader) {
        this.portReader = portReader;
        this.portReader.addListener(this);
    }

    public static void upload(final MemoryRoms memoryRoms) throws InterruptedException, IOException {
        final String headerFilename = FilenameUtils.removeExtension(memoryRoms.getHeaderFilename());
        final Properties props = Runner.getProperties();
        final String portName = props.getProperty(PROP_PORT_NAME);
        final int baudRate = parseHexInt(props.getProperty(PROP_BAUD_RATE));
        final UploadHelper helper = new UploadHelper(PortReader.createPortReader(portName, baudRate));
        helper.blockMap.clear();
        final String fileName = Path.of(props.getProperty(PROP_PATCH), headerFilename + ".hex").toString();
        helper.createSerialBlocks(fileName);
        helper.waitForSubmit();
        log.info("helper.portReader.stop()");
        helper.portReader.stop();
    }

    /**
     * Upload single file from UI panel
     *
     * @param portReader открытый serial ports
     * @throws InterruptedException on ...
     * @throws IOException          on ...
     */
    public static void uploadFile(final PortReader portReader) throws InterruptedException, IOException {
        /* todo: надо убрать ссылки на Runner, Properties... */
        final Properties prop = Runner.getProperties();
        final String fileName = FilenameUtils.removeExtension((new File(
            prop.getProperty(PROP_PATCH), prop.getProperty(PROP_DATA_FILE)
        )).getPath()) + ".hex";

        final UploadHelper helper = new UploadHelper(portReader);
        helper.blockMap.clear();
        helper.createSerialBlocks(fileName);
        helper.waitForSubmit();
    }

    private void createSerialBlocks(final String fileName) throws IOException {
        log.info("Hex Dump File = \"{}\"", fileName);
        try (
            final BufferedReader reader = new BufferedReader(
                new InputStreamReader(
                    new FileInputStream(fileName), Charset.forName("windows-1251")
                )
            )
        ) {
            String line;
            byte device = 0;
            short address = 0;
            while ((line = reader.readLine()) != null) {
                if (line.startsWith("@")) {
                    String prefix = "";
                    final String[] spl = StringUtils.split(line.substring(1), ',');
                    for (final String param : spl) {
                        final String[] splitParam = StringUtils.split(param, ':');
                        switch (splitParam[0]) {
                            case "prefix":
                                prefix = splitParam[1];
                                break;
                            case "device":
                                device = parseHexByte(splitParam[1]);
                                break;
                            case "address":
                                address = parseHexShort(splitParam[1]);
                                break;
                        }
                    }
                    log.info("смена { prefix: {}, device: {}, address: {} }", prefix, device, address);
                } else if (line.startsWith(":")) {
                    final byte[] body = DatatypeConverter.parseHexBinary(line.substring(1));
                    final byte size = (byte) body.length;
                    final SerialBlock block = new SerialBlock();
                    block.setDevice(device);
                    block.setBody(body);
                    block.setSize(size);
                    block.setAddress(address);
                    blockMap.put(DeviceAddress.of(device, address), block);
                    log.info("[{},{}]{}", device, address, DatatypeConverter.printHexBinary(body));
                    address += size;
                }
            }
        }
    }

    @Override
    public void portReaderCarriageReturn(final String str) {
        if (str.startsWith(":adr=")) {
            final String[] spl = StringUtils.split(str.substring(5), ",");
            final byte device = parseByte(spl[0]);
            final short address = parseHexShort(spl[1]);
            final SerialBlock block = blockMap.get(DeviceAddress.of(device, address));
            if (block != null) {
                block.setUploaded(true);
            }
        }
        log.info("From Arduino: {}", str);
    }

    private void waitForSubmit() throws InterruptedException {
        portReader.writeString("\n");
        boolean hasNotUploaded = true;
        do {
            for (int i = 0; i < 5 && hasNotUploaded; i++) {
                log.info("waitForSubmit i:{}", i);
                Thread.sleep(1000);
                hasNotUploaded = blockMap.values().stream().anyMatch(serialBlock -> !serialBlock.isUploaded());
            }
            blockMap.values().forEach(serialBlock -> {
                if (!serialBlock.isUploaded()) {
                    log.info("serialBlock[{},{}] set processed true", serialBlock.getDevice(), serialBlock.getAddress());
                    serialBlock.setProcessed(true);
                }
            });
            portReader.writeString("\n");
        } while (hasNotUploaded);
        portReader.removeListener(this);
    }

    @Override
    public void portReaderTrash(final byte[] data) {
//        log.info("Reader Trash: \"{}\"", DatatypeConverter.printHexBinary(data));
    }

    @Override
    public void portWriterRun() {
        try {
            for (final SerialBlock block : blockMap.values()) {
                if (block.isProcessed()) {
                    final byte[] dump = block.getWriteBytes();
                    log.info("hex[{},{}] = \"{}\"", block.getDevice(), block.getAddress(), DatatypeConverter.printHexBinary(dump));
                    portReader.writeByte(dump, dump.length);
                    block.setProcessed(false);
                    Thread.sleep(100);
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
        final int baudRate = parseHexInt(prop.getProperty(PROP_BAUD_RATE));

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
