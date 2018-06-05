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

import javax.xml.bind.DatatypeConverter;
import java.util.Properties;

import static com.lasgis.arduino.eeprom.Runner.PROP_BAUD_RATE;
import static com.lasgis.arduino.eeprom.Runner.PROP_PORT_NAME;

/**
 * Загружаем дамп в arduino.
 *
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Slf4j
public class UploadHelper implements PortReaderListener {

    public static void upload() throws InterruptedException {
        final Properties prop = Runner.getProperties();
        final String portName = prop.getProperty(PROP_PORT_NAME);
        final int baudRate = Integer.valueOf(prop.getProperty(PROP_BAUD_RATE));
        final UploadHelper helper = new UploadHelper();
        final PortReader portReader = PortReader.createPortReader(portName, baudRate);
        portReader.addListener(helper);
        Thread.sleep(1000);
        helper.upload(portReader);
    }

    private void upload(final PortReader portReader) throws InterruptedException {
        for (int i = 0; i < 150; i++) {
            final SerialBlock sb = new SerialBlock();
            sb.body = ("проверка связи [" + i + "]").getBytes(RomData.CHARSET);
            sb.setSize((byte) sb.getBody().length);
            sb.setAddress((short) (i * 10));
            sb.setCs((short) i);
            final byte[] dump = sb.getBytes();
            log.info("hex Out = \"{}\"", DatatypeConverter.printHexBinary(dump));
            log.info("\"{}\"", new String(dump, RomData.CHARSET));
            portReader.writeByte(dump, dump.length);
        }
        portReader.stop();
    }

    @Override
    public void portReaderCarriageReturn(final String string) {
        log.info("From Arduino: \"{}\"", string);
    }

    @Override
    public void portReaderTrash(final String string) {
//        log.info("Reader Trash: \"{}\"", string);
    }
}
