/*
 *  @(#)SettingMenuItem.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.util;

import lombok.AllArgsConstructor;
import lombok.Getter;

import java.awt.event.ActionListener;

/**
 * Класс для предварительного заполнения параметров для создания меню.
 * @author VLaskin
 * @version 1.0
 * Date: 14.01.2005
 * Time: 15:19:18
 */
@Getter
@AllArgsConstructor
public class SettingMenuItem {
    /** текст в меню. */
    private final String text;
    /** иконка в меню. */
    private final String image;
    /** всплывающая подсказка. */
    private final String toolTip;
    /** действие на кнопку. */
    private final ActionListener action;
    /** список подменю для данного меню. */
    private final SettingMenuItem[] items;
}
