/**
 * @(#)PortReaderListener.java Title: DA CME Web Application Description: Программная система DA "Ассистент Доктора".
 * Входит в состав  КМЭ - Комплекс медицинский экспертный. Copyright (c) 2016 CME CWISS AG Company. All Rights
 * Reserved.
 */

package com.lasgis.robot.control.serial;

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
