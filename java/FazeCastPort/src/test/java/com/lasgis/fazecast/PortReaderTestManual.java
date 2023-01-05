/*
 *  @(#)PortReaderTest.java  last: 06.01.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.fazecast;

import lombok.extern.slf4j.Slf4j;
import org.junit.jupiter.api.Test;

/**
 * The Class PortReaderTest definition.
 *
 * @author Vladimir Laskin
 * @since 06.01.2023 : 1:56
 */
@Slf4j
class PortReaderTestManual {

    @Test
    void createPortReader() {
        try {
            final PortReaderTestRun testRun = new PortReaderTestRun();
            for (int i = 0; i < 5; i++) {
                Thread.sleep(10000);
                testRun.reader.writeString("проверка связи");
            }
            Thread.sleep(50000);
            testRun.reader.stop();
        } catch (final Exception ex) {
            log.error(ex.getMessage(), ex);
        }

    }
}
