/*
 *  @(#)SerialPortWrap.java  last: 08.01.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.serial;

import com.fazecast.jSerialComm.SerialPort;
import lombok.Getter;

/**
 * The Class SerialPortWrap definition.
 *
 * @author Vladimir Laskin
 * @since 08.01.2023 : 14:50
 */
@Getter
public class SerialPortWrap {
    private final SerialPort serialPort;
    public SerialPortWrap(final SerialPort serialPort) {
        this.serialPort = serialPort;
    }

    @Override
    public String toString() {
        return serialPort.getDescriptivePortName();
    }
}
