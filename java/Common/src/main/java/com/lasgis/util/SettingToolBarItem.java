/*
 *  @(#)SettingToolBarItem.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.util;

import java.awt.event.ActionListener;

/**
 * Класс для предварительного заполнения параметров для создания панели
 * инструментов.
 * @author VLaskin
 * @version 1.0
 * Date: 17.01.2005
 * Time: 11:55:05
 */
public class SettingToolBarItem {

    /** текст на кнопке. */
    private String text;
    /** Ссылка на файл изображения на кнопке. */
    private String image;
    /** всплывающая подсказка. */
    private String toolTip;
    /** ширина кнопки. */
    private int width;
    /** высота кнопки. */
    private int height;
    /** Событие, которое проявляется при нажатии на кнопку. */
    private ActionListener action;

    /**
     * Создаём новую кнопку.
     * @param aText текст на кнопке
     * @param aImage изображение на кнопке
     * @param aToolTip всплывающая подсказка
     * @param aWidth ширина кнопки
     * @param aHeight высота кнопки
     * @param aAction some aAction events
     */
    public SettingToolBarItem(
        String aText,
        String aImage,
        String aToolTip,
        int aWidth,
        int aHeight,
        ActionListener aAction
    ) {
        this.text = aText;
        this.image = aImage;
        this.toolTip = aToolTip;
        this.width = aWidth;
        this.height = aHeight;
        this.action = aAction;
    } //SettingToolBarItem(String, String, String, int, int, ActionListener)

    /**
     * Вернуть текст на кнопке.
     * @return текст на кнопке
     */
    public String getText() {
        return text;
    }

    /**
     * Вернуть Ссылку на файл изображения на кнопке.
     * @return Ссылку
     */
    public String getImage() {
        return image;
    }

    /**
     * Вернуть всплывающую подсказку.
     * @return всплывающую подсказку
     */
    public String getToolTip() {
        return toolTip;
    }

    /**
     * Вернуть ширину кнопки.
     * @return ширину кнопки
     */
    public int getWidth() {
        return width;
    }

    /**
     * Вернуть высоту кнопки.
     * @return высоту кнопки
     */
    public int getHeight() {
        return height;
    }

    /**
     * Вернуть Событие, которое проявляется при нажатии на кнопку.
     * @return Событие
     */
    public ActionListener getAction() {
        return action;
    }
}
