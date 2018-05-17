/*
 *  @(#)PortReaderListener.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.robot.serial;

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
     * @param string часть фарша
     */
    void portReaderTrash(String string);
}
