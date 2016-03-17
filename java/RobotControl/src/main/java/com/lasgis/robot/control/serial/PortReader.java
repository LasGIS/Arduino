/**
 * @(#)PortReader.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2015 LasGIS Company. All Rights Reserved.
 */

package com.lasgis.robot.control.serial;

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

    private static SerialPort serialPort;

    /**
     * Конструктор.
     */
    public PortReader() {
        //Передаём в конструктор имя порта
        serialPort = new SerialPort("COM14");
        try {
            //Открываем порт
            serialPort.openPort();
            //Выставляем параметры
            serialPort.setParams(SerialPort.BAUDRATE_9600,
                                 SerialPort.DATABITS_8,
                                 SerialPort.STOPBITS_1,
                                 SerialPort.PARITY_NONE);
            //Включаем аппаратное управление потоком
            serialPort.setFlowControlMode(SerialPort.FLOWCONTROL_RTSCTS_IN |
                                          SerialPort.FLOWCONTROL_RTSCTS_OUT);
            //Устанавливаем ивент лисенер и маску
            serialPort.addEventListener(new PortReader(), SerialPort.MASK_RXCHAR);
            //Отправляем запрос устройству
            serialPort.writeString("Get data 1");
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
                LOG.info(data);
                //И снова отправляем запрос
                serialPort.writeString("Get data");
            } catch (final SerialPortException ex) {
                LOG.error(ex.getMessage(), ex);
            }
        }
    }
}
