/*
 *  @(#)UpLoad.java  last: 30.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom;

import com.lasgis.arduino.eeprom.memory.RomARRAY;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.arduino.eeprom.memory.RomOBJECT;
import com.lasgis.util.ByteArrayBuilder;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.io.FilenameUtils;
import org.apache.commons.lang.StringUtils;

import javax.xml.bind.DatatypeConverter;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.text.MessageFormat;
import java.util.Enumeration;
import java.util.List;
import java.util.Properties;

/**
 * Запуск UpLoad.
 *
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Slf4j
public class UpLoad {

    private static final Properties prop = new Properties();

    private final static String TAB = "  ";

    private final static String PROP_PORT_NAME = "port.name";
    private final static String PROP_BAUD_RATE = "baud.rate";
    private final static String PROP_PATCH     = "patch";
    private final static String PROP_DATA_FILE = "data";

    private final static String[] MANDATORY_PROPERTY_KEYS = {
        PROP_PORT_NAME, PROP_BAUD_RATE, PROP_PATCH, PROP_DATA_FILE
    };

    /**
     * Загружаем свойства и выполняем логику
     * на входе следующие параметры:
     *   -port.name COM3
     *   -baud.rate 96000
     *   -patch D:\Arduino\arduino\SmartClock20
     *   -data I2CMemory.data
     *   и / или
     *   -prop SmartClock20.properties
     * который содержит все эти параметры
     * @param args аргументы командной строки
     */
    private UpLoad(final String[] args) throws Exception {
        loadProp(args);
        showProp();
        validateProp();
        final List<RomData> array = loadDataFile();
        final ByteArrayBuilder bab = new ByteArrayBuilder();
        for (final RomData item : array) {
            item.toEeprom(bab);
        }
        log.info("\n byte[] = \"" + DatatypeConverter.printHexBinary(bab.toByte()) + "\"");
        for (final RomData item : array) {
            showNameOffset(item);
        }
    }

    private List<RomData> loadDataFile() throws Exception {
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
                return DataCppLoader.load(dataFile);
            }
            default:
                throw new UpLoadInfoException("Расширение {} не поддерживается", extension);
        }
    }

    private void showNameOffset(final RomData rom) {
        final StringBuilder sb = new  StringBuilder("\n");
        showNameOffset(rom, sb, "");
        log.info(sb.toString());
    }

    private void showNameOffset(final RomData rom, final StringBuilder sb, final String tab) {
        final String name = rom.getName();
        if (StringUtils.isNotBlank(name)) {
            sb.append(tab).append("name: ").append(rom.getName()).append("; ");
        } else {
            sb.append(tab);
        }
        sb.append("offset: ").append(rom.getOffset()).append(";");
        if (rom instanceof RomOBJECT) {
            sb.append(" {\n");
            for (final RomData inst: ((RomOBJECT) rom).getArray()) {
                showNameOffset(inst, sb, tab + TAB);
            }
            sb.append(tab).append("}\n");
        } else if (rom instanceof RomARRAY) {
            sb.append(" [\n");
            for (final RomData inst: ((RomARRAY) rom).getArray()) {
                showNameOffset(inst, sb, tab + TAB);
            }
            sb.append(tab).append("]\n");
        } else {
            sb.append("\n");
        }
    }

    /**
     * Загружаем свойства
     * @param args аргументы командной строки
     * @throws IOException если ошибка
     */
    private void loadProp(final String[] args) throws IOException {
        final Properties properties = new Properties();
        String name = "undefine";
        for (final String option : args) {
            if (option.startsWith("-")) {
                name = option.substring(1).trim();
            } else {
                properties.setProperty(name, option);
            }
        }
        final String fileName = properties.getProperty("prop");
        if (fileName != null) {
            final FileInputStream is = new FileInputStream(fileName);
            prop.load(is);
            is.close();
        }
        prop.putAll(properties);
    }

    /**
     * Показываем свойства в логе
     */
    private void showProp() {
        Enumeration<Object> keys = prop.keys();
        while (keys.hasMoreElements()) {
            final String key = (String) keys.nextElement();
            final String val = prop.getProperty(key);
            log.info("{}={}", key, val);
        }
    }

    /**
     * Проверяем все-ли свойства прочитаны
     * @throws Exception если ошибка
     */
    private void validateProp() throws Exception {
        for (final String key : MANDATORY_PROPERTY_KEYS) {
            if (!prop.containsKey(key)) {
                throw new UpLoadInfoException(
                    MessageFormat.format("Пропущен обязательный параметр \"{0}\"", key)
                );
            }
        }
        final File file = new File(prop.getProperty(PROP_PATCH), prop.getProperty(PROP_DATA_FILE));
        if (!file.exists() || !file.isFile()) {
            throw new UpLoadInfoException(
                MessageFormat.format("Файла \"{0}\" не существует!", file.getPath())
            );
        }
    }

    /**
     * Главный запуск программы.
     * @param args аргументы командной строки
     */
    public static void main(final String[] args) {
        try {
            new UpLoad(args);
        } catch (final UpLoadInfoException ex) {
            log.error(ex.getMessage());
        } catch (final Exception ex) {
            log.error(ex.getMessage(), ex);
        }
    }
}
