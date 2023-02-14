/*
 *  @(#)LoadHelper.java  last: 15.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.load;

import com.lasgis.arduino.eeprom.CommonInfoException;
import com.lasgis.arduino.eeprom.Runner;
import com.lasgis.arduino.eeprom.memory.BatchMemory;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.util.ByteArrayBuilder;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.io.FilenameUtils;

import java.io.File;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
import java.util.Properties;

import static com.lasgis.arduino.eeprom.Runner.PROP_DATA_FILE;
import static com.lasgis.arduino.eeprom.Runner.PROP_PATCH;

/**
 * Диспетчер по чтению входных данных.
 *
 * @author Vladimir Laskin
 * @since 31.05.2018
 */
@Slf4j
public class LoadHelper {

    /**
     * компиляция исходных данных
     * @return список полученных RomData объектов
     * @throws Exception on ...
     */
    public static List<BatchMemory> load() throws Exception {
        final Properties prop = Runner.getProperties();
        final File dataFile = new File(
            prop.getProperty(PROP_PATCH),
            prop.getProperty(PROP_DATA_FILE)
        );
        final String extension = FilenameUtils.getExtension(dataFile.getPath());
        switch (extension) {
            case "xml": {
                return DataXmlLoader.load(dataFile);
            }
            case "data": {
                // todo надо переделать: DataCppLoader.load(dataFile) должен возвращать List<BatchMemory>
                final List<BatchMemory> roms = new ArrayList<>();
                final BatchMemory batch = BatchMemory.of();
                batch.setRomDataList(DataCppLoader.load(dataFile));
                roms.add(batch);
                return roms;
            }
            default:
                throw new CommonInfoException("Расширение {} не поддерживается", extension);
        }
    }

    /**
     * Создаём образ дампа
     * @return образ дампа
     * @throws UnsupportedEncodingException on ...
     */
    public static byte[] createDump() throws UnsupportedEncodingException {
        final ByteArrayBuilder bab = new ByteArrayBuilder();
        // todo заглушка - переделать
        final List<RomData> romDataList = Runner.getDataList().get(0).getRomDataList();
        for (final RomData item : romDataList) {
            item.toEeprom(bab);
        }
        return bab.toByte();
    }
}
