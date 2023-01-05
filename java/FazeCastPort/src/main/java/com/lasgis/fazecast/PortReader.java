/*
 *  @(#)PortReader.java  last: 05.01.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.fazecast;

import com.fazecast.jSerialComm.SerialPort;
import com.fazecast.jSerialComm.SerialPortDataListener;
import com.fazecast.jSerialComm.SerialPortEvent;
import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;

/**
 * The Class PortReader.
 *
 * @author Vladimir Laskin
 * @version 1.0
 */
@Slf4j
public class PortReader implements SerialPortDataListener {

    private final static Charset cp1251 = Charset.forName("Cp1251");
    @Getter
    private static PortReader portReader = null;
    @Getter
    private SerialPort serialPort;

    private final StringBuilder sb = new StringBuilder();
    private final List<PortReaderListener> listeners = new ArrayList<>();

    /**
     * Конструктор.
     */
    private PortReader(final String portName, final int baudRate) {
        //Передаём в конструктор имя порта
        serialPort = SerialPort.getCommPort(portName);

        //Открываем порт
        serialPort.openPort();
        //Выставляем параметры
        serialPort.setComPortParameters(baudRate, 8, SerialPort.ONE_STOP_BIT, SerialPort.NO_PARITY);
        //Устанавливаем ивент лисенер
        serialPort.addDataListener(this);
        portReader = this;
    }

    public static PortReader createPortReader(final String portName, final int baudRate) {
        if (portReader != null) {
            portReader.stop();
        }
        portReader = new PortReader(portName, baudRate);
        return portReader;
    }

    /**
     * Отправляем message устройству
     *
     * @param data message
     */
    public void writeString(final String data) {
        if (portReader != null && portReader.serialPort != null) {
            final byte[] bytes = data.getBytes(cp1251);
            portReader.serialPort.writeBytes(bytes, bytes.length);
        }
    }

    /**
     * Отправляем message устройству
     *
     * @param dump message
     */
    public void writeByte(final byte[] dump, final long len) throws InterruptedException {
        if (portReader != null && portReader.serialPort != null) {
            portReader.serialPort.writeBytes(dump, len);
        }
    }

    public void stop() {
        if (serialPort != null) {
            serialPort.closePort();
            serialPort = null;
        }
    }

    @Override
    public int getListeningEvents() {
        return SerialPort.LISTENING_EVENT_DATA_AVAILABLE |
            SerialPort.LISTENING_EVENT_DATA_RECEIVED;
    }

    @Override
    public void serialEvent(final SerialPortEvent event) {
        //Получаем ответ от устройства, обрабатываем данные и т.д.
        if ((event.getEventType() & SerialPort.LISTENING_EVENT_DATA_RECEIVED) != 0x0) {
            final byte[] data = event.getReceivedData();
            log.info("ReceivedData: {}", new String(data, cp1251));
        }
    }

    public void addListener(final PortReaderListener listener) {
        listeners.add(listener);
    }
}
