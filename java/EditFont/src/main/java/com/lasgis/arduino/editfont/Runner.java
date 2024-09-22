/*
 *  @(#)Runner.java  last: 22.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont;

import com.lasgis.arduino.editfont.data.FontDataPerformed;
import com.lasgis.arduino.editfont.load.LoadHelper;
import com.lasgis.arduino.editfont.panels.ControlHelper;
import com.lasgis.arduino.editfont.test.TestHelper;
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
import java.util.concurrent.TimeUnit;

/**
 * The Class Runner definition.
 *
 * @author Vladimir Laskin
 * @since 25.08.2024 : 17:36
 */
@Slf4j
public class Runner {
    public final static String PROP_PATCH = "patch";
    public final static String PROP_FONT_C_FILE = "font_c";
    public final static String PROP_FONT_H_FILE = "font_h";
    public final static String PROP_FONT_KEY = "font_key";
    @Getter
    private static final Properties properties = new Properties();
    @Getter
    private static final List<CommandType> commands = new ArrayList<>();
    private final static String[] MANDATORY_PROPERTY_KEYS = {
        PROP_PATCH, PROP_FONT_C_FILE, PROP_FONT_H_FILE, PROP_FONT_KEY
    };

    /**
     * <pre>
     * Загружаем свойства и выполняем логику
     * на входе следующие параметры:
     *   -patch C:\Arduino\java\EditFont\data\
     *   -font_c Font16.c
     *   -font_h Font16.h
     * и / или
     *   -properties Font.properties
     * который содержит все эти параметры.
     *
     * Список команд указываются в конце командной строки (без дефиса в префиксе)
     *   test     - тестовый прогон
     *   panel    - Открываем окно и все команды выполняем в интерактивном режиме
     * </pre>
     *
     * @param args аргументы командной строки
     */
    public Runner(final String[] args) throws Exception {
        loadProp(args);
        showProp();
        if (!commands.contains(CommandType.panel)) {
            validateProp();
        }

        if (commands.contains(CommandType.test) || commands.contains(CommandType.panel)) {
            LoadHelper.load();
        }

        /* далее обработка по командам */
        if (commands.contains(CommandType.test)) {
            TestHelper.show();
            FontDataPerformed.stop(10, TimeUnit.SECONDS);
        } else {
/*
        if (commands.contains(CommandType.create)) {
            CreateHelper.create(properties.getProperty(PROP_PATCH), memoryRoms);
        }
        if (commands.contains(CommandType.upload)) {
            UploadHelper.upload(memoryRoms);
        }
        if (commands.contains(CommandType.read)) {
            UploadHelper.read();
        }
*/
            if (commands.contains(CommandType.panel)) {
                ControlHelper.panel();
            }
        }
    }

    /**
     * Загружаем свойства
     *
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
     */
    private void validateProp() {
        for (final String key : MANDATORY_PROPERTY_KEYS) {
            if (!properties.containsKey(key)) {
                throw new CommonInfoException(
                    MessageFormat.format("Пропущен обязательный параметр \"{0}\"", key)
                );
            }
        }
        final File file = new File(properties.getProperty(PROP_PATCH), properties.getProperty(PROP_FONT_C_FILE));
        if (!file.exists() || !file.isFile()) {
            throw new CommonInfoException(
                MessageFormat.format("Файла \"{0}\" не существует!", file.getPath())
            );
        }
    }
}
