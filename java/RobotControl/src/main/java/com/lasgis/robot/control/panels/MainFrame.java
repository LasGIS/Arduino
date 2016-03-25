/**
 * @(#)MainFrame.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2015 LasGIS Company. All Rights Reserved.
 */

package com.lasgis.robot.control.panels;

import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.JToolBar;
import java.awt.AWTEvent;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

import com.lasgis.component.StatusBar;
import com.lasgis.util.SettingMenuItem;
import com.lasgis.util.SettingToolBarItem;
import com.lasgis.util.Util;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Created by IntelliJ IDEA.
 *
 * @author VLaskin
 * @version 1.0
 */
public class MainFrame extends JFrame implements ComponentListener {

    private static final Logger LOG = LoggerFactory.getLogger(MainFrame.class);
    private static final Color WATCH_FOR_SELECTED_ON = new Color(200, 150, 150);
    private static final Color WATCH_FOR_SELECTED_OFF = new Color(240, 240, 240);

    /** размеры строки состояния. */
    private static final int[] STATUS_BAR_SIZES = new int[] {0, 100, 200};
    /** Строка состояния. */
    private StatusBar jStatusBar = new StatusBar(STATUS_BAR_SIZES);
    /** Панель с картой. */
    private MapPanel mapPanel = new MapPanel();
    /** панель конфигурации. */
    private ConfigPanel configPanel;

    /** ширина кнопки на главной панели инструментов. */
    private static final int TOOL_BAR_WIDTH = 27;
    /** высота кнопки на главной панели инструментов. */
    private static final int TOOL_BAR_HEIGHT = 27;

    /** Настройка главного меню. */
    private final SettingMenuItem[] menuSetting = {
        new SettingMenuItem(
            "File", "openFile.gif", "", null,
            new SettingMenuItem[] {
                new SettingMenuItem(
                    "Exit", null, "Закрываем приложение", MainFrame.this::jMenuFileExitAction, null
                ),
            }
        ),
        new SettingMenuItem(
            "Help", "help.gif", "Всякого рода вспоможение", null,
            new SettingMenuItem[] {
                new SettingMenuItem(
                    "About", "help.gif", "Кто ЭТО сделал!", MainFrame.this::jMenuHelpAboutAction, null
                )
            }
        )
    };

    /** Настройка главной панели инструментов. */
    private final SettingToolBarItem[] toolBarSetting = {
        new SettingToolBarItem(
            "Помощь", "help.gif", "Help",
            TOOL_BAR_WIDTH, TOOL_BAR_HEIGHT, MainFrame.this::jMenuHelpAboutAction
        ),
        new SettingToolBarItem(
            "Exit", null, "Exit from programm",
            TOOL_BAR_WIDTH, TOOL_BAR_HEIGHT, MainFrame.this::jMenuFileExitAction
        )
    };

    /**
     * Construct the frame.
     */
    public MainFrame() {
        enableEvents(AWTEvent.WINDOW_EVENT_MASK);
        try {
            final Dimension size = new Dimension(800, 600);
            final JPanel contentPane = (JPanel) this.getContentPane();
            contentPane.setLayout(new BorderLayout());
            this.setSize(size);
            this.setTitle("RobotControl ROBOT support");

            /* настраиваем главное меню */
            final JMenuBar menuBar = new JMenuBar();
            for (SettingMenuItem aSetMenu : menuSetting) {
                menuBar.add(Util.createImageMenuItem(aSetMenu));
            }
            /* настраиваем главный ToolBar */
            final JToolBar toolBar = new JToolBar();
            for (SettingToolBarItem aSetToolBar : toolBarSetting) {
                toolBar.add(Util.createImageButton(aSetToolBar));
            }
            /* разделительная панелька */
            final JSplitPane splitPane = new JSplitPane();
            splitPane.setContinuousLayout(true);
            mapPanel.setMainFrame(this);
            mapPanel.addComponentListener(this);
            //Make textField get the focus whenever frame is activated.
            this.addWindowFocusListener(new WindowAdapter() {
                public void windowGainedFocus(final WindowEvent e) {
                    mapPanel.requestFocusInWindow();
                }
            });
            this.setJMenuBar(menuBar);
            contentPane.add(toolBar, BorderLayout.NORTH);
            contentPane.add(jStatusBar, BorderLayout.SOUTH);
            contentPane.add(splitPane, BorderLayout.CENTER);
            configPanel = new ConfigPanel();
            configPanel.setMainFrame(this);

            splitPane.add(configPanel, JSplitPane.RIGHT);
            splitPane.add(mapPanel, JSplitPane.LEFT);
            //splitPane.setLastDividerLocation(size.width - 300);
            //splitPane.setDividerLocation(size.width - 300);
            splitPane.setResizeWeight(1);
        } catch (final Exception ex) {
            LOG.error(ex.getMessage(), ex);
        }
    } // constructor::MainFrame()

    /**
     * File | Exit action performed.
     *
     * @param event Action Event
     */
    public void jMenuFileExitAction(final ActionEvent event) {
        // сохраняем локальную конфигурацию
        System.exit(0);
    }

    /**
     * Help | About action performed.
     * @param event Action Event
     */
    public void jMenuHelpAboutAction(final ActionEvent event) {
        final MainFrameAboutBox dlg = new MainFrameAboutBox(this);
        final Dimension dlgSize = dlg.getPreferredSize();
        final Dimension frmSize = getSize();
        final Point loc = getLocation();
        dlg.setLocation(
            (frmSize.width - dlgSize.width) / 2 + loc.x,
            (frmSize.height - dlgSize.height) / 2 + loc.y
        );
        dlg.setModal(true);
        dlg.pack();
        dlg.setVisible(true);
    }

    /**
     * Overridden so we can exit when window is closed.
     * @param e оконное событие
     */
    protected void processWindowEvent(final WindowEvent e) {
        super.processWindowEvent(e);
/*
        if (e.getID() == WindowEvent.WINDOW_CLOSING) {
        }
*/
    }

    /**
     * Вывод сообщений на statusBar.
     * @param out строка сообщения
     * @param numItem номер элемента статусной строки
     */
    public void outStatus(final String out, final int numItem) {
        jStatusBar.setText(out, numItem);
    }

    /**
     * Вернуть панель с картой.
     * @return панель с картой
     */
    public MapPanel getMapPanel() {
        return mapPanel;
    }

    /**
     * Вернуть панель конфигурации.
     * @return панель конфигурации
     */
    public ConfigPanel getConfigPanel() {
        return configPanel;
    }

    /**
     * Invoked when the component's size changes.
     * @param e event which indicates that a component moved
     */
    public void componentResized(final ComponentEvent e) {
        if (e.getComponent().equals(mapPanel)) {
            mapPanel.setRedrawMap(true);
        }
    }

    /**
     * Invoked when the component's position changes.
     * @param e event which indicates that a component moved
     */
    public void componentMoved(final ComponentEvent e) {

    }

    /**
     * Invoked when the component has been made visible.
     * @param e event which indicates that a component moved
     */
    public void componentShown(final ComponentEvent e) {

    }

    /**
     * Invoked when the component has been made invisible.
     * @param e event which indicates that a component moved
     */
    public void componentHidden(final ComponentEvent e) {

    }
}