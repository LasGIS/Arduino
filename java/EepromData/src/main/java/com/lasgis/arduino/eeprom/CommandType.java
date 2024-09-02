/*
 *  @(#)CommandType.java  last: 02.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom;

/**
 * Возможные типы и названия команд обработки.
 *
 * @author Vladimir Laskin
 * @since 31.05.2018
 */
public enum CommandType {
    /** Вывод тестовой информации о дампе */
    test,
    /**
     * <pre>
     * создание файлов:
     *   Memory.h    - определители (#define ...)
     *   Memory.hex  - образ памяти
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
