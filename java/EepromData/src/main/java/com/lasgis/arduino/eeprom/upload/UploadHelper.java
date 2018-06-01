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

    private final static UploadHelper helper = new UploadHelper();

    public static void upload() throws InterruptedException {
        final Properties prop = Runner.getProperties();
        final String portName = prop.getProperty(PROP_PORT_NAME);
        final int baudRate = Integer.valueOf(prop.getProperty(PROP_BAUD_RATE));
        final PortReader portReader = PortReader.createPortReader(portName, baudRate);
        portReader.addListener(helper);
        for (int i = 0; i < 150; i++) {
            //PortReader.writeString("connection test [" + i + "]\n");
            //Thread.sleep(200);
            final byte[] dump = ("проверка связи [" + i + "] qwerty qwerty qwerty qwerty12345\n").getBytes(RomData.CHARSET);
            PortReader.writeByte(dump, dump.length);
        }
        Thread.sleep(2000);
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
