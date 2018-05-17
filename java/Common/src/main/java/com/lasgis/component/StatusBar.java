/*
 *  @(#)StatusBar.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.component;

import javax.swing.*;
import java.awt.*;

/**
 * Строка состояния, состоящая из нескольких разделов.
 *
 * @author vlaskin
 * @version 1.0
 * @since 18.03.2008 : 14:15:04
 */
public class StatusBar extends JComponent {

    /** размер разделителя между элементами. */
    private static final int SIZE_WEIGHT_DELEMITER = 5;

    /** размер разделителя между элементами. */
    public static final int SIZE_HEIGHT_STATUSBAR = 24;

    /**
     * конструктор строки.
     */
    public StatusBar() {
        this(new int[] {0, 100, 100, 100});
    }

    /**
     * конструктор строки.
     * @param sizes число разделов
     */
    public StatusBar(final int[] sizes) {
        super();
        setLayout(new BoxLayout(this, BoxLayout.LINE_AXIS));
        add(new StatusBarItem(" ", 0));
        add(Box.createHorizontalGlue());
        for (int i = 1; i < sizes.length; i++) {
            add(new StatusBarItem(" ", sizes[i]));
            Box.Filler rigidArea = (Box.Filler) Box.createRigidArea(
                new Dimension(
                    SIZE_WEIGHT_DELEMITER,
                    SIZE_HEIGHT_STATUSBAR
                )
            );
            rigidArea.setForeground(new Color(0xff, 0xff, 0xff));
            add(rigidArea);
        }
    }

    /**
     * Указать новое значение для первого раздела.
     * @param text значение
     */
    public void setText(final String text) {
        setText(text, 0);
    }

    /**
     * Указать новое значение для конкретного раздела.
     * @param text значение
     * @param numItem номер элемента
     */
    public void setText(final String text, final int numItem) {
        final JLabel label = (JLabel) this.getComponent(numItem * 2);
        label.setText(StatusBarItem.stringDeNormalize(text));
    }

}