/*
 *  @(#)PopupListener.java  last: 25.08.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.panels;

import javax.swing.JPopupMenu;
import java.awt.Point;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

/**
 * @author vlaskin
 * @version 1.0 from May 2, 2006 : 8:35:36 PM
 */
public class PopupListener extends MouseAdapter {

    /**
     * Запоминаем позицию мышки.
     */
    private static Point mousePoint;
    private final JPopupMenu popup;

    PopupListener(final JPopupMenu popupMenu) {
        popup = popupMenu;
    }

    @Override
    public void mousePressed(final MouseEvent e) {
        maybeShowPopup(e);
    }

    @Override
    public void mouseReleased(final MouseEvent e) {
        maybeShowPopup(e);
    }

    private void maybeShowPopup(final MouseEvent e) {
        mousePoint = e.getPoint();
        if (e.isPopupTrigger()) {
            popup.show(e.getComponent(), e.getX(), e.getY());
        }
    }
}
