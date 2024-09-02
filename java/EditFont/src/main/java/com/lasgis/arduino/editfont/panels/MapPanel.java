/*
 *  @(#)MapPanel.java  last: 03.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.panels;

import lombok.Setter;
import lombok.extern.slf4j.Slf4j;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.FocusEvent;
import java.awt.event.FocusListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.awt.image.BufferedImage;
import java.util.concurrent.atomic.AtomicBoolean;


/**
 * Окно вывода карты.
 *
 * @author VLaskin
 * @version 1.0
 */
@Slf4j
public class MapPanel extends JPanel
    implements MouseMotionListener, KeyListener, FocusListener, MouseListener, MouseWheelListener {

    /** Серый цвет фона. */
    public static final Color PANEL_GRAY_COLOR = new Color(220, 220, 220);

    /** Цвет фона для выбранной панели. */
    public static final Color PANEL_FOCUSED_COLOR = new Color(255, 220, 220);

    /** Светло-серый цвет фона. */
    public static final Color PANEL_LIGHT_GRAY_COLOR = new Color(240, 240, 240);

    /** Текущая координата мышки X для показа в инфо. */
    int currentXScreen;

    /** Текущая координата мышки Y для показа в инфо. */
    int currentYScreen;

    /** Если false, то принципиально не перерисовываем. */
    AtomicBoolean isAutoDraw = new AtomicBoolean(true);

    /** Ссылка на MainFrame. */
    @Setter
    private MainFrame mainFrame = null;

    /** Если true, то будем перегружать. */
    private boolean isRedrawMap = true;

    /** Сохраненное изображение. */
    private BufferedImage grBackgroundImage = null;

    /** Признак, что панель в фокусе. */
    private boolean focused = false;

    /**
     * Конструктор.
     */
    public MapPanel() {
        try {
            setBackground(Color.white);
            setBorder(BorderFactory.createLoweredBevelBorder());
            setLayout(new BorderLayout());
            addMouseMotionListener(this);
            addMouseListener(this);
            addMouseWheelListener(this);
            addKeyListener(this);
            addFocusListener(this);
            setFocusable(true);
        } catch (final Exception ex) {
            log.error(ex.getMessage(), ex);
        }
    }

    private void addElement(final ActionEvent actionEvent) {

    }

    /**
     * Стандартный вход для рисования.
     *
     * @param gr контекст вывода
     */
    public void paint(final Graphics gr) {
        final Dimension dim = getSize();
        if (focused) {
            gr.setColor(MapPanel.PANEL_FOCUSED_COLOR);
        } else {
            gr.setColor(MapPanel.PANEL_GRAY_COLOR);
        }
        gr.fillRect(0, 0, dim.width, dim.height);

        if (isRedrawMap || grBackgroundImage == null) {
            grBackgroundImage = new BufferedImage(
                dim.width, dim.height, BufferedImage.TYPE_INT_RGB
            );
            final Graphics bckGr = grBackgroundImage.getGraphics();
            bckGr.setColor(MapPanel.PANEL_GRAY_COLOR);
            bckGr.fillRect(0, 0, dim.width, dim.height);
            /* тест out */
            bckGr.setColor(MapPanel.PANEL_LIGHT_GRAY_COLOR);
            bckGr.drawLine(1, 1, 100, 100);
            bckGr.drawLine(1, 100, 100, 1);
            isRedrawMap = false;
        }
        gr.drawImage(grBackgroundImage, 0, 0, dim.width, dim.height, null);
        //drawing.grid();
    }

    /**
     * Print some message for out in status box.
     *
     * @param out     string for message
     * @param numItem номер элемента статусной строки
     */
    private void outStatus(final String out, final int numItem) {
        if (mainFrame != null) {
            mainFrame.outStatus(out, numItem);
        }
    }

    /**
     * Invoked when a key has been typed.
     *
     * @param e An event which indicates that a keystroke occurred in a component.
     */
    public void keyTyped(final KeyEvent e) {
    }

    /**
     * Invoked when a key has been pressed.
     *
     * @param e An event which indicates that a keystroke occurred in a component.
     */
    public void keyPressed(final KeyEvent e) {
        final Dimension dim = this.getBounds().getSize();
        double multi;
        if (e.isControlDown()) {
            multi = 0.3;
        } else {
            multi = 0.1;
        }
        switch (e.getKeyCode()) {
            case KeyEvent.VK_NUMPAD4:
            case KeyEvent.VK_LEFT:
                /* влево */
                isRedrawMap = true;
                break;
            case KeyEvent.VK_NUMPAD6:
            case KeyEvent.VK_RIGHT:
                /* вправо */
                isRedrawMap = true;
                break;
            case KeyEvent.VK_NUMPAD8:
            case KeyEvent.VK_UP:
                /* вверх */
                isRedrawMap = true;
                break;
            case KeyEvent.VK_NUMPAD2:
            case KeyEvent.VK_DOWN:
                /* вниз */
                isRedrawMap = true;
                break;
            case KeyEvent.VK_DIVIDE:
                /* уменьшить масштаб */
                isRedrawMap = true;
                break;
            case KeyEvent.VK_MULTIPLY:
                /* увеличить масштаб */
                isRedrawMap = true;
                break;
            default:
                break;
        }
        if (isRedrawMap) {
            showCoordinates(dim.width / 2, dim.height / 2);
            this.repaint();
        }
    }

    /**
     * Invoked when a key has been released.
     *
     * @param e An event which indicates that a keystroke occurred in a component.
     */
    public void keyReleased(final KeyEvent e) {
    }

    /**
     * Invoked when the mouse wheel is rotated.
     *
     * @param e MouseWheelEvent
     * @see MouseWheelEvent
     */
    public void mouseWheelMoved(final MouseWheelEvent e) {
        final Dimension dim = this.getBounds().getSize();
        //final int modifier = e.getModifiersEx();
        final int rotation = e.getWheelRotation();
        Point mousePnt = e.getPoint();

        //isRedrawMap = true;
        showCoordinates(mousePnt.x, mousePnt.y);
        this.repaint();
    }

    @Override
    public void mouseDragged(final MouseEvent e) {
    }

    @Override
    public void mouseMoved(final MouseEvent e) {
        showCoordinates(e.getX(), e.getY());
    }

    /**
     * Выводим координаты в StatusString.
     *
     * @param xScreen экранная координата X
     * @param yScreen экранная координата Y
     */
    public void showCoordinates(final int xScreen, final int yScreen) {
        // save current coordinates
        currentXScreen = xScreen;
        currentYScreen = yScreen;
        String sb = xScreen + "  " +
            yScreen;
        outStatus(sb, 2);
    }

    /**
     * Invoked when a mouse button has been pressed on a component.
     *
     * @param e MouseEvent
     */
    public void mousePressed(final MouseEvent e) {
        if (e.getButton() == MouseEvent.BUTTON1) {
            boolean isRedraw = false;
            final Rectangle rec = this.getBounds();
            final Point mousePnt = e.getPoint();

/*
            if ((mousePnt.x < rec.x + SIZE_BORDER)
                || (mousePnt.x > rec.x + rec.width - SIZE_BORDER)
                || (mousePnt.y < rec.y + SIZE_BORDER)
                || (mousePnt.y > rec.y + rec.height - SIZE_BORDER)
            ) {
                // Попали в обрамляющее поле. Переходим по стрелке
                final Point centerPnt = new Point(
                    (rec.x + rec.width) / 2,
                    (rec.y + rec.height) / 2
                );
                outStatus("Move v", 1);
                requestFocusInWindow();
                isRedrawMap = true;
                isRedraw = true;
            } else {
                // Попали в поле карты
            }
*/
            if (isRedraw) {
                showCoordinates(mousePnt.x, mousePnt.y);
                repaint();
            }
        }
    }

    /**
     * Invoked when a mouse button has been released on a component.
     *
     * @param e MouseEvent
     */
    public void mouseReleased(final MouseEvent e) {
//        outStatus(e.paramString());
    }

    /**
     * Invoked when the mouse button has been clicked (pressed and released) on a component.
     *
     * @param e MouseEvent
     */
    public void mouseClicked(final MouseEvent e) {
//        outStatus(e.paramString());
    }

    /**
     * Invoked when the mouse enters a component.
     *
     * @param e MouseEvent
     */
    public void mouseEntered(final MouseEvent e) {
//        outStatus(e.paramString());
    }

    /**
     * Invoked when the mouse exits a component.
     *
     * @param e MouseEvent
     */
    public void mouseExited(final MouseEvent e) {
//        outStatus(e.paramString());
    }

    /**
     * Invoked when a component gains the keyboard focus.
     *
     * @param e A low-level event
     */
    public void focusGained(final FocusEvent e) {
        focused = true;
        final boolean oldRedrawMap = isRedrawMap;
        isRedrawMap = false;
        repaint();
        isRedrawMap = oldRedrawMap;
    }

    /**
     * Invoked when a component loses the keyboard focus.
     *
     * @param e A low-level event
     */
    public void focusLost(final FocusEvent e) {
        focused = false;
        final boolean oldRedrawMap = isRedrawMap;
        isRedrawMap = false;
        repaint();
        isRedrawMap = oldRedrawMap;
    }

    /**
     * Устанавливаем признак перерисовки.
     *
     * @param redrawMap if true, then redraw component
     */
    public void setRedrawMap(final boolean redrawMap) {
        this.isRedrawMap = redrawMap;
    }

    /**
     * Устанавливаем autoDraw.
     *
     * @param autoDraw autoDraw
     */
    public void setAutoDraw(final boolean autoDraw) {
        isAutoDraw.getAndSet(autoDraw);
    }
}
