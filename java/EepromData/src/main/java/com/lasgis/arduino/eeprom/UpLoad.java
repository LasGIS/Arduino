/*
 *  @(#)UpLoad.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom;

import lombok.extern.slf4j.Slf4j;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.text.MessageFormat;
import java.util.Enumeration;
import java.util.Properties;

/**
 * Запуск UpLoad.
 *
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Slf4j
public class UpLoad {

    public static final Properties prop = new Properties();

    public final static String PROP_PORT_NAME = "port.name";
    public final static String PROP_BAUD_RATE = "baud.rate";
    public final static String PROP_PATCH     = "patch";
    public final static String PROP_DATA_FILE = "data";

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
