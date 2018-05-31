/*
 *  @(#)CommandType.java  last: 31.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom;

/**
 * Возможные типы и названия команд обработки.
 *
 * @author Vladimir Laskin
 * @since 31.05.2018
 */
public enum CommandType {
    /** вывод тестовой информации о дампе */
    test,
    /**
     * создание файлов:
     *   I2CMemory.h    - определители (#define ...)
     *   I2CMemory.hex  - образ памяти
     */
    create,
    /**
     * Загрузка образа памяти в arduino
     * Эту команду можно запускать отдельно (без чтения исходных файлов и создание образа)
     */
    upload
}
