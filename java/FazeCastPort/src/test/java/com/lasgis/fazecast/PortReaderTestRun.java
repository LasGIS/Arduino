/*
 *  @(#)PortReaderTestRun.java  last: 05.01.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.fazecast;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

/**
 * The Class PortReaderTestRun definition.
 *
 * @author Vladimir Laskin
 * @since 05.01.2023 : 23:56
 */
@Slf4j
@Getter
public class PortReaderTestRun implements PortReaderListener {
    public final PortReader reader;
    public PortReaderTestRun() {
        reader = PortReader.createPortReader("COM4", 9600);
        reader.addListener(this);
    }

    @Override
    public void portReaderCarriageReturn(final String string) {
        log.info(string);
    }

    @Override
    public void portReaderTrash(final String string) {
        log.info(string);
    }
}
