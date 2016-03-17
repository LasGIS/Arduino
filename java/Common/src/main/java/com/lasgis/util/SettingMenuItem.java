/*
 * @(#)SettingMenuItem.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2013 LasGIS Company. All Rights Reserved.
 */

package com.lasgis.util;

import java.awt.event.ActionListener;

/**
 * Класс для предварительного заполнения параметров для создания меню.
 * @author VLaskin
 * @version 1.0
 * Date: 14.01.2005
 * Time: 15:19:18
 */
public class SettingMenuItem {

    /** текст в меню. */
    private String text;
    /** иконка в меню. */
    private String image;
    /** всплывающая подсказка. */
    private String toolTip;
    /** действие на кнопку. */
    private ActionListener action;
    /** список подменю для данного меню. */
    private SettingMenuItem[] items = null;

    /**
     * Создаём экземпляр класса для предварительного заполнения параметров для создания меню.
     * @param aText текст меню
     * @param aImage иконка меню
     * @param aToolTip всплывающая подсказка
     * @param aAction действие на кнопку
     * @param aItems SettingMenuItem[] список подменю для данного меню
     */
    public SettingMenuItem(
        String aText,
        String aImage,
        String aToolTip,
        ActionListener aAction,
        SettingMenuItem[] aItems
    ) {
        this.text = aText;
        this.image = aImage;
        this.toolTip = aToolTip;
        this.action = aAction;
        this.items = aItems;
    }

    /**
     * Get текст меню.
     * @return текст меню
     */
    public String getText() {
        return text;
    }

    /**
     * Get иконка меню.
     * @return иконка меню
     */
    public String getImage() {
        return image;
    }

    /**
     * Get всплывающая подсказка.
     * @return всплывающая подсказка
     */
    public String getToolTip() {
        return toolTip;
    }

    /**
     * Get действие на кнопку.
     * @return действие на кнопку
     */
    public ActionListener getAction() {
        return action;
    }

    /**
     * Get список подменю для данного меню.
     * @return список подменю для данного меню
     */
    public SettingMenuItem[] getItems() {
        return items;
    }

}
/*
 $Log: SettingMenuItem.java,v $
 Revision 1.2  2005/01/17 06:44:24  VLaskin
 Create tools for simple create toolbar

 Revision 1.1  2005/01/14 11:15:32  VLaskin
 Create tools for simple create menus
*/
