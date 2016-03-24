/**
 * @(#)PortReader.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2015 LasGIS Company. All Rights Reserved.
 */

package com.lasgis.robot.control.serial;

import java.util.ArrayList;
import java.util.List;

import jssc.SerialPort;
import jssc.SerialPortEvent;
import jssc.SerialPortEventListener;
import jssc.SerialPortException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * The Class PortReader.
 * @author Vladimir Laskin
 * @version 1.0
 */
public class PortReader implements SerialPortEventListener {

    private static final Logger LOG = LoggerFactory.getLogger(PortReader.class);

    private SerialPort serialPort = null;
    private static PortReader portReader = null;

    final StringBuilder sb = new StringBuilder();
    final List<PortReaderListener> listeners = new ArrayList<>();

    /**
     * Конструктор.
     */
    private PortReader(final String portName, final int baudRate) {
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
            /*serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN |
                                          SerialPort.FLOWCONTROL_RTSCTS_OUT);*/
            //Устанавливаем ивент лисенер и маску
            serialPort.addEventListener(this, SerialPort.MASK_RXCHAR);
            portReader = this;

        } catch (final SerialPortException ex) {
            LOG.error(ex.getMessage(), ex);
        }
    }

    public static PortReader createPortReader(final String portName, final int baudRate) {

        if (portReader != null && portReader.serialPort != null) {
            try {
                portReader.serialPort.closePort();
            } catch (final SerialPortException ex) {
                LOG.error(ex.getMessage(), ex);
            }
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
    public static void writeString(final String data) {
        try {
            final PortReader portReader = PortReader.getPortReader();
            if (portReader != null) {
                if (portReader.serialPort != null) {
                    portReader.serialPort.writeString(data);
                }
            }
        } catch (final SerialPortException ex) {
            LOG.error(ex.getMessage(), ex);
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
                    //LOG.info(string);
                    sb.delete(0, crInd + 2);
                }
                for (final PortReaderListener listener : listeners) {
                    listener.portReaderTrash(data);
                }
                System.out.print(data);
            } catch (final SerialPortException ex) {
                LOG.error(ex.getMessage(), ex);
            }
        }
    }

    public void addListener(final PortReaderListener listener) {
        listeners.add(listener);
    }
}
