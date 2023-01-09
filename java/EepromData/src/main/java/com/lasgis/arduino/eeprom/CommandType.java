/*
 *  @(#)CommandType.java  last: 09.01.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
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
     * <pre>
     * создание файлов:
     *   I2CMemory.h    - определители (#define ...)
     *   I2CMemory.hex  - образ памяти
     * </pre>
     */
    create,
    /**
     * Загрузка образа памяти в arduino
     * Эту команду можно запускать отдельно (без чтения исходных файлов и создание образа)
     */
    upload,
    /** Чтение образа памяти из arduino */
    read,
    /** Открываем окно и все команды выполняем в интерактивном режиме */
    panel
}
