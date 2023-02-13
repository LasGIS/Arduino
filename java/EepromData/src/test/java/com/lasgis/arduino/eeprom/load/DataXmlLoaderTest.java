/*
 *  @(#)DataXmlLoaderTest.java  last: 13.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */
package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.memory.RomData;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.net.URL;
import java.util.List;

/**
 * The Class DataXmlLoaderTest definition.
 *
 * @author Vladimir Laskin
 * @since 12.02.2023 : 0:40
 */
class DataXmlLoaderTest {
    @Test
    void load() throws Exception {
        ClassLoader classLoader = getClass().getClassLoader();
        URL resource = classLoader.getResource("TestMemory.xml");
        assert resource != null;
        final File dataFile = new File(resource.toURI());
        List<RomData> romData = DataXmlLoader.load(dataFile);
        Assertions.assertNotNull(romData);
    }
}
