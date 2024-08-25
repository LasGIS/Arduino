/*
 *  @(#)EditFontApplication.java  last: 25.08.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont;

import lombok.extern.slf4j.Slf4j;

/**
 * Запуск Edit Font Application.
 *
 * @author Vladimir Laskin
 * @since 31.05.2018
 */
@Slf4j
public class EditFontApplication {

    /**
     * Главный запуск программы.
     *
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
