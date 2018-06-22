/*
 * @(#)Runner.java
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright © 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom;

import com.lasgis.arduino.eeprom.create.CreateHelper;
import com.lasgis.arduino.eeprom.load.LoadHelper;
import com.lasgis.arduino.eeprom.memory.RomData;
import com.lasgis.arduino.eeprom.test.TestHelper;
import com.lasgis.arduino.eeprom.upload.UploadHelper;
import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.Properties;

/**
 * Запуск EepromData.
 *
 * @author Vladimir Laskin
 * @since 31.05.2018
 */
@Slf4j
public class Runner {

    @Getter
    private static final Properties properties = new Properties();
    @Getter
    private static final List<CommandType> commands = new ArrayList<>();
    @Getter
    private static List<RomData> dataList;
    @Getter
    private static byte[] dump;

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
     * и / или
     *   -properties SmartClock20.properties
     * который содержит все эти параметры.
     *
     * Список команд указываются в конце командной строки (без дефиса в префиксе)
     *   test     - вывод тестовой информации о дампе
     *   create   - создание файлов:
     *      I2CMemory.h    - определители (#define ...)
     *      I2CMemory.hex  - образ памяти
     *   upload   - загрузка образа памяти в arduino
     *              Эту команду можно запускать отдельно (без чтения исходных файлов и создание образа)
     * @param args аргументы командной строки
     */
    private Runner(final String[] args) throws Exception {
        loadProp(args);
        showProp();
        validateProp();

        if (commands.contains(CommandType.test) ||
            commands.contains(CommandType.create)) {
            dataList = LoadHelper.load();
            dump = LoadHelper.createDump();
        }

        /* далее обработка по командам */
        if (commands.contains(CommandType.test)) {
            TestHelper.show();
        }
        if (commands.contains(CommandType.create)) {
            CreateHelper.create();
        }
        if (commands.contains(CommandType.upload)) {
            UploadHelper.upload();
        }
        if (commands.contains(CommandType.read)) {
            UploadHelper.read();
        }
    }

    /**
     * Загружаем свойства
     * @param args аргументы командной строки
     * @throws IOException если ошибка
     */
    private void loadProp(final String[] args) throws IOException {
        final Properties properties = new Properties();
        String name = null;
        for (final String option : args) {
            if (option.startsWith("-")) {
                name = option.substring(1).trim();
            } else {
                if (name != null) {
                    properties.setProperty(name, option);
                    name = null;
                } else {
                    commands.add(CommandType.valueOf(option));
                }
            }
        }
        final String fileName = properties.getProperty("properties");
        if (fileName != null) {
            final FileInputStream is = new FileInputStream(fileName);
            Runner.properties.load(is);
            is.close();
        }
        Runner.properties.putAll(properties);
    }

    /**
     * Показываем свойства в логе
     */
    private void showProp() {
        Enumeration<Object> keys = properties.keys();
        while (keys.hasMoreElements()) {
            final String key = (String) keys.nextElement();
            final String val = properties.getProperty(key);
            log.info("{}={}", key, val);
        }
        log.info("Список команд: {}", commands);
    }

    /**
     * Проверяем все-ли свойства прочитаны
     * @throws Exception если ошибка
     */
    private void validateProp() throws Exception {
        for (final String key : MANDATORY_PROPERTY_KEYS) {
            if (!properties.containsKey(key)) {
                throw new CommonInfoException(
                    MessageFormat.format("Пропущен обязательный параметр \"{0}\"", key)
                );
            }
        }
        final File file = new File(properties.getProperty(PROP_PATCH), properties.getProperty(PROP_DATA_FILE));
        if (!file.exists() || !file.isFile()) {
            throw new CommonInfoException(
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
            new Runner(args);
        } catch (final CommonInfoException ex) {
            log.error(ex.getMessage());
        } catch (final Exception ex) {
            log.error(ex.getMessage(), ex);
        }
    }
}
