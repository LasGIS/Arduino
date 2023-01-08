/*
 *  @(#)PortReaderListener.java  last: 05.01.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.serial;

/**
 * Обработчик данных из device.
 * @author Vladimir Laskin
 * @version 1.0
 */
public interface PortReaderListener {

    /**
     * Получаем и обрабатываем данные от устройства.
     * Обработка только после получения всей строки
     * @param string строка для обработки без символа перевода строки
     */
    void portReaderCarriageReturn(String string);

    /**
     * Получаем весь хлам по частям
     * @param data часть фарша
     */
    void portReaderTrash(byte[] data);
}
