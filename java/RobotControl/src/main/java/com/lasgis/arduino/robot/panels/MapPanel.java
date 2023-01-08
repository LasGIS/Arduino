/*
 *  @(#)MapPanel.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.robot.panels;

import com.lasgis.serial.PortReaderListener;
import com.lasgis.util.SettingMenuItem;
import com.lasgis.util.Util;
import lombok.extern.slf4j.Slf4j;

import javax.swing.*;
import javax.xml.bind.DatatypeConverter;
import java.awt.*;
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
 * @author VLaskin
 * @version 1.0
 */
@Slf4j
public class MapPanel extends JPanel
    implements MouseMotionListener, KeyListener, FocusListener, MouseListener, MouseWheelListener, PortReaderListener {

    /** серый цвет фона. */
    public static final Color PANEL_GRAY_COLOR = new Color(220, 220, 220);
    /** цвет фона для выбранной панели. */
    public static final Color PANEL_FOCUSED_COLOR = new Color(255, 220, 220);
    /** светло-серый цвет фона. */
    public static final Color PANEL_LIGHT_GRAY_COLOR = new Color(240, 240, 240);
    /** Размер зарамочного оформления. */
    private static final int SIZE_BORDER = 15;
    /** ссылка на MainFrame. */
    private MainFrame mainFrame = null;

    /** Если true, то будем перегружать. */
    private boolean isRedrawMap = true;
    /** сохраненное изображение. */
    private BufferedImage grBackgroundImage = null;
    /** признак, что панель в фокусе. */
    private boolean focused = false;
    /** Текущая мышковая координата X для показа в инфо. */
    int currentXScreen;
    /** Текущая мышковая координата Y для показа в инфо. */
    int currentYScreen;
    /** Если false, то принципиально не перерисовываем. */
    AtomicBoolean isAutoDraw = new AtomicBoolean(true);

    /** Настройка выпадающего меню (пока только заготовка). */
    private SettingMenuItem[] setPopUpMenu = {
        new SettingMenuItem(
            "Добавить животное", "openFile.gif", "Добавить выделенное животное в данное место",
            MapPanel.this::addElement, null
        ),
    };

    private void addElement(final ActionEvent actionEvent) {

    }

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
            createPopupMenu();
        } catch (final Exception ex) {
            log.error(ex.getMessage(), ex);
        }
    }

    /**
     * стандартный вход для рисования.
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
        gr.draw3DRect(
            SIZE_BORDER,
            SIZE_BORDER,
            dim.width - (SIZE_BORDER * 2) - 1,
            dim.height - (SIZE_BORDER * 2) - 1,
            false
        );
        gr.setColor(MapPanel.PANEL_GRAY_COLOR);
        gr.fillRect(
            SIZE_BORDER + 1,
            SIZE_BORDER + 1,
            dim.width - (SIZE_BORDER * 2) - 2,
            dim.height - (SIZE_BORDER * 2) - 2
        );
        gr.clipRect(
            SIZE_BORDER + 1,
            SIZE_BORDER + 1,
            dim.width - (SIZE_BORDER * 2) - 2,
            dim.height - (SIZE_BORDER * 2) - 2
        );
        if (isRedrawMap || grBackgroundImage == null) {
            grBackgroundImage = new BufferedImage(
                dim.width, dim.height, BufferedImage.TYPE_INT_RGB
            );
            final Graphics bckGr = grBackgroundImage.getGraphics();
            bckGr.setColor(MapPanel.PANEL_GRAY_COLOR);
            bckGr.fillRect(0, 0, dim.width, dim.height);
            isRedrawMap = false;
        }
        gr.drawImage(grBackgroundImage, 0, 0, dim.width, dim.height, null);
        //drawing.grid();
    }

    /**
     * Print some message for out in status box.
     * @param out string for message
     * @param numItem номер элемента статусной строки
     */
    private void outStatus(final String out, final int numItem) {
        if (mainFrame != null) {
            mainFrame.outStatus(out, numItem);
        }
    }

    /**
     * Установить добавить ссылку на главное окно.
     * @param mainFrame главное окно
     */
    public void setMainFrame(final MainFrame mainFrame) {
        this.mainFrame = mainFrame;
    }

    /**
     * Invoked when a key has been typed.
     * @param e An event which indicates that a keystroke occurred in
     * a component.
     */
    public void keyTyped(final KeyEvent e) {
    }

    /**
     * Invoked when a key has been pressed.
     * @param e An event which indicates that a keystroke occurred in
     * a component.
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
            case KeyEvent.VK_NUMPAD4 :
            case KeyEvent.VK_LEFT :
                // влево
                //isRedrawMap = true;
                break;
            case KeyEvent.VK_NUMPAD6:
            case KeyEvent.VK_RIGHT:
                // вправо
                //isRedrawMap = true;
                break;
            case KeyEvent.VK_NUMPAD8:
            case KeyEvent.VK_UP:
                // вверх
                //isRedrawMap = true;
                break;
            case KeyEvent.VK_NUMPAD2:
            case KeyEvent.VK_DOWN:
                // вниз
                //isRedrawMap = true;
                break;
            case KeyEvent.VK_DIVIDE:
                // уменьшить масштаб
                //isRedrawMap = true;
                break;
            case KeyEvent.VK_MULTIPLY:
                // увеличить масштаб
                //isRedrawMap = true;
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
     * @param e An event which indicates that a keystroke occurred in
     * a component.
     */
    public void keyReleased(final KeyEvent e) {
    }

    /**
     * Invoked when the mouse wheel is rotated.
     * @see MouseWheelEvent
     * @param e MouseWheelEvent
     */
    public void mouseWheelMoved(final MouseWheelEvent e) {
        final Dimension dim = this.getBounds().getSize();
        //final int modif = e.getModifiersEx();
        final int rotation = e.getWheelRotation();
        Point mouspnt = e.getPoint();

        //isRedrawMap = true;
        showCoordinates(mouspnt.x, mouspnt.y);
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
     * @param xScreen экранная координата X
     * @param yScreen экранная координата Y
     */
    public void showCoordinates(final int xScreen, final int yScreen) {
        // save current coordinates
        currentXScreen = xScreen;
        currentYScreen = yScreen;
        final StringBuilder sb = new StringBuilder();
        sb.append(xScreen).append("  ");
        sb.append(yScreen);
        outStatus(sb.toString(), 2);
    }

    /**
     * Invoked when a mouse button has been pressed on a component.
     * @param e MouseEvent
     */
    public void mousePressed(final MouseEvent e) {
        if (e.getButton() == MouseEvent.BUTTON1) {
            boolean isRedraw = false;
            final Rectangle rec = this.getBounds();
            final Point mouspnt = e.getPoint();

            if ((mouspnt.x < rec.x + SIZE_BORDER)
                || (mouspnt.x > rec.x + rec.width - SIZE_BORDER)
                || (mouspnt.y < rec.y + SIZE_BORDER)
                || (mouspnt.y > rec.y + rec.height - SIZE_BORDER)
            ) {
                // Попали в обрамляющее поле. Переходим по стрелке
                final Point cntrpnt = new Point(
                    (rec.x + rec.width) / 2,
                    (rec.y + rec.height) / 2
                );
                //outStatus("Move v");
                requestFocusInWindow();
                isRedrawMap = true;
                isRedraw = true;
            //} else {
                // Попали в поле карты
            }
            if (isRedraw) {
                showCoordinates(mouspnt.x, mouspnt.y);
                repaint();
            }
        }
    }

    /**
     * Invoked when a mouse button has been released on a component.
     * @param e MouseEvent
     */
    public void mouseReleased(final MouseEvent e) {
//        outStatus(e.paramString());
    }

    /**
     * Invoked when the mouse button has been clicked (pressed
     * and released) on a component.
     * @param e MouseEvent
     */
    public void mouseClicked(final MouseEvent e) {
//        outStatus(e.paramString());
    }

    /**
     * Invoked when the mouse enters a component.
     * @param e MouseEvent
     */
    public void mouseEntered(final MouseEvent e) {
//        outStatus(e.paramString());
    }

    /**
     * Invoked when the mouse exits a component.
     * @param e MouseEvent
     */
    public void mouseExited(final MouseEvent e) {
//        outStatus(e.paramString());
    }

    /**
     * создаём и настраиваем выпадающее меню.
     */
    public void createPopupMenu() {

        // создаём и настраиваем выпадающее меню
        final JPopupMenu popup = new JPopupMenu();
        for (SettingMenuItem aSetMenu : setPopUpMenu) {
            popup.add(Util.createImageMenuItem(aSetMenu));
        }

        //Add listener to the text area so the popup menu can come up.
        final MouseListener popupListener = new PopupListener(popup);
        this.addMouseListener(popupListener);
    }

    /**
     * Invoked when a component gains the keyboard focus.
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
     * @param redrawMap if true, then redraw component
     */
    public void setRedrawMap(final boolean redrawMap) {
        this.isRedrawMap = redrawMap;
    }

    /**
     * Устанавливаем autoDraw.
     * @param autoDraw autoDraw
     */
    public void setAutoDraw(final boolean autoDraw) {
        isAutoDraw.getAndSet(autoDraw);
    }

    @Override
    public void portReaderCarriageReturn(final String string) {

    }

    @Override
    public void portReaderTrash(final byte[] data) {

    }
}
