/*
 *  @(#)SettingToolBarItem.java  last: 09.01.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.util;

import lombok.AllArgsConstructor;
import lombok.Getter;

import java.awt.event.ActionListener;

/**
 * Класс для предварительного заполнения параметров для создания панели
 * инструментов.
 * @author VLaskin
 * @version 1.0
 * Date: 17.01.2005
 * Time: 11:55:05
 */
@Getter
@AllArgsConstructor
public class SettingToolBarItem {
    /** текст на кнопке. */
    private final String text;
    /** Ссылка на файл изображения на кнопке. */
    private final String image;
    /** всплывающая подсказка. */
    private final String toolTip;
    /** ширина кнопки. */
    private final int width;
    /** высота кнопки. */
    private final int height;
    /** Событие, которое проявляется при нажатии на кнопку. */
    private final ActionListener action;
}
