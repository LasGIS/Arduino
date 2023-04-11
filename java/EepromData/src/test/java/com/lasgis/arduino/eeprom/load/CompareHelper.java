/*
 *  @(#)CompareHelper.java  last: 11.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load;

import lombok.extern.slf4j.Slf4j;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

import static java.util.Objects.nonNull;

/**
 * <description>
 *
 * @author vlaskin
 * @since <pre>11.04.2023</pre>
 */
@Slf4j
public class CompareHelper {
    protected boolean compareDefinitionFile(final String targetPath, final String expectedFilename, final String actualFilename) throws Exception {
        final File expectedFile = new File(targetPath, expectedFilename);
        final File actualFile = new File(targetPath, actualFilename);
        boolean isFail = false;
        try (
            final BufferedReader expected = new BufferedReader(new FileReader(expectedFile));
            final BufferedReader actual = new BufferedReader(new FileReader(actualFile));
        ) {
            String expectedLine = expected.readLine();
            String actualLine = actual.readLine();
            int count = 0;
            log.info("{}:", actualFilename);
            while (nonNull(expectedLine) || nonNull(actualLine)) {
                count++;
//                Assertions.assertEquals(expectedLine, actualLine);
                if (!(nonNull(expectedLine) && expectedLine.equals(actualLine))) {
                    log.info("({}) Expected >{}", count, expectedLine);
                    log.info("     Actual   >{}", actualLine);
                    isFail = true;
                }
                expectedLine = expected.readLine();
                actualLine = actual.readLine();
            }
        }
        return isFail;
    }
}
