/*
 * @(#)StatusBarItem.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2013 LasGIS Company. All Rights Reserved.
 */

package com.lasgis.component;

import javax.swing.JLabel;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;
import java.awt.Dimension;

/**
 * Элемент status bara.
 *
 * @author vlaskin
 * @version 1.0
 * @since 18.03.2008 : 19:33:57
 */
public class StatusBarItem extends JLabel {

    /** */
    private Dimension size = null;

    /**  */
    private static Border border = new BevelBorder(BevelBorder.LOWERED);

    /**
     *
     */
    private StatusBarItem() {
        super();
    }

    /**
     * Конструктор.
     * @param str начальное значение строки
     * @param aSize размер по вертикали
     */
    public StatusBarItem(String str, int aSize) {
        super(stringDeNormalize(str));
        if (aSize > 0) {
            this.setBorder(border);
            this.size = new Dimension(aSize, StatusBar.SIZE_HEIGHT_STATUSBAR);
        }
    }

    /**
     * Добавляем пробел спереди для лучшей визуализации.
     * @param str входная строка
     * @return ненормализованная выходная строка
     */
    public static String stringDeNormalize(String str) {
        if (str == null || (str.length() == 0)) {
            return " ";
        } else {
            return " " + str;
        }
    }

    /**
     * Возвращаем предпочтительный размер ячейки (если он есть).
     * @return the value of the <code>preferredSize</code>
     * @see javax.swing.plaf.ComponentUI
     */
    public Dimension getPreferredSize() {
        if (size != null) {
            return size;
        } else {
            return super.getPreferredSize();
        }
    }

    /**
     * Возвращаем минимальный размер ячейки (если он есть).
     * @return the value of the <code>MinimumSize</code>
     * @see javax.swing.plaf.ComponentUI
    public java.awt.Dimension getMinimumSize() {
        if (size != null) {
            return size;
        } else {
            return super.getMinimumSize();
        }
    }
     */

}