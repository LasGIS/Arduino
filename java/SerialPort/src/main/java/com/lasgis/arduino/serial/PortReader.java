/*
 *  @(#)PortReader.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.serial;

import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;
import lombok.extern.slf4j.Slf4j;

import java.util.ArrayList;
import java.util.List;

/**
 * The Class PortReader.
 * @author Vladimir Laskin
 * @version 1.0
 */
@Slf4j
public class PortReader implements SerialPortEventListener {

    private static PortReader portReader = null;
    private SerialPort serialPort = null;

    private final StringBuilder sb = new StringBuilder();
    private final List<PortReaderListener> listeners = new ArrayList<>();

    /**
     * Конструктор.
     */
    private PortReader(final String portName, final int baudRate) {
        connect(portName, baudRate);
    }

    public void connect(final String portName, final int baudRate) {
        try {
            //Передаём в конструктор имя порта
            serialPort = new SerialPort(portName);

            //Открываем порт
            serialPort.openPort();
            //Выставляем параметры
            serialPort.setParams(
                baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE
            );
            //Включаем аппаратное управление потоком
            /* serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN | SerialPort.FLOWCONTROL_RTSCTS_OUT); */
            //Устанавливаем ивент лисенер и маску
            serialPort.addEventListener(this, SerialPort.MASK_RXCHAR);
            portReader = this;

        } catch (final SerialPortException ex) {
            log.error(ex.getMessage(), ex);
        }
    }

    public static PortReader createPortReader(final String portName, final int baudRate) {

        if (portReader != null) {
            portReader.stop();
        }

        portReader = new PortReader(portName, baudRate);
        return portReader;
    }

    public static PortReader getPortReader() {
        return portReader;
    }

    /**
     * Отправляем message устройству
     * @param data message
     */
    public void writeString(final String data) {
        try {
            if (portReader != null) {
                if (portReader.serialPort != null) {
                    portReader.serialPort.writeString(data);
                }
            }
        } catch (final SerialPortException ex) {
            log.error(ex.getMessage(), ex);
        }
    }

    /**
     * Отправляем message устройству
     * @param dump message
     */
    public void writeByte(final byte[] dump, final int len) throws InterruptedException {
        try {
            if (portReader != null && portReader.serialPort != null) {
                for (int i = 0; i < dump.length && i < len; i++) {
                    portReader.serialPort.writeByte(dump[i]);
                    Thread.sleep(20);
                }
            }
        } catch (final SerialPortException ex) {
            log.error(ex.getMessage(), ex);
        }
    }

    public void stop() {
        if (serialPort != null) {
            try {
                serialPort.closePort();
            } catch (final SerialPortException ex) {
                log.error(ex.getMessage(), ex);
            }
            serialPort = null;
        }
    }

    @Override
    public void serialEvent(final SerialPortEvent event) {
        if (event.isRXCHAR() && event.getEventValue() > 0) {
            try {
                //Получаем ответ от устройства, обрабатываем данные и т.д.
                final String data = serialPort.readString(event.getEventValue());
                sb.append(data);
                int crInd;
                while ((crInd = sb.indexOf("\r\n")) >= 0) {
                    final String string = sb.substring(0, crInd);
                    for (final PortReaderListener listener : listeners) {
                        listener.portReaderCarriageReturn(string);
                    }
                    //log.info(string);
                    sb.delete(0, crInd + 2);
                }
                for (final PortReaderListener listener : listeners) {
                    listener.portReaderTrash(data);
                }
//                System.out.print(data);
            } catch (final SerialPortException ex) {
                log.error(ex.getMessage(), ex);
            }
        }
    }

    public void addListener(final PortReaderListener listener) {
        listeners.add(listener);
    }

    public SerialPort getSerialPort() {
        return serialPort;
    }
}
