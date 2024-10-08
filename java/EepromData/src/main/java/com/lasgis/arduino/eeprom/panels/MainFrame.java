/*
 *  @(#)MainFrame.java  last: 03.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.panels;

import com.lasgis.arduino.eeprom.Runner;
import com.lasgis.component.StatusBar;
import com.lasgis.util.SettingMenuItem;
import com.lasgis.util.SettingToolBarItem;
import com.lasgis.util.Util;
import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.JToolBar;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.AWTEvent;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.Properties;

import static com.lasgis.arduino.eeprom.Runner.PROP_PATCH;

/**
 * MainFrame class
 *
 * @author VLaskin
 * @version 1.0
 */
@Slf4j
public class MainFrame extends JFrame implements ComponentListener {

    /** ширина кнопки на главной панели инструментов. */
    private static final int TOOL_BAR_WIDTH = 27;
    /** высота кнопки на главной панели инструментов. */
    private static final int TOOL_BAR_HEIGHT = 27;

    /** Строка состояния. */
    private final StatusBar jStatusBar = new StatusBar(new int[]{0, 100, 200});
    /** Панель с картой. */
    @Getter
    private final MapPanel mapPanel = new MapPanel();
    /** Панель конфигурации. */
    @Getter
    private final ConfigPanel configPanel = new ConfigPanel();

    /** Настройка главного меню. */
    private final SettingMenuItem[] menuSetting = {
        new SettingMenuItem(
            "File", "openFile.gif", "", null,
            new SettingMenuItem[]{
                new SettingMenuItem(
                    "Exit", "exit.png", "Закрываем приложение", this::jMenuFileExitAction, null
                ),
            }
        ),
        new SettingMenuItem(
            "Help", "userGuide.png", "Всякого рода вспоможение", null,
            new SettingMenuItem[]{
                new SettingMenuItem(
                    "About", "help.gif", "Кто ЭТО сделал!", this::jMenuHelpAboutAction, null
                )
            }
        )
    };

    /** Настройка главной панели инструментов. */
    private final SettingToolBarItem[] toolBarSetting = {
        new SettingToolBarItem(
            "Create", "create.png", "Создаем по файлу",
            TOOL_BAR_WIDTH, TOOL_BAR_HEIGHT, this::jMenuCreateHexDefinitionAction
        ),
        new SettingToolBarItem(
            "Upload", "upload.png", "Запись блока",
            TOOL_BAR_WIDTH, TOOL_BAR_HEIGHT, this::jMenuUploadFileAction
        ),
        new SettingToolBarItem(
            "Помощь", "userGuide.png", "Help",
            TOOL_BAR_WIDTH, TOOL_BAR_HEIGHT, this::jMenuHelpAboutAction
        ),
        new SettingToolBarItem(
            "Exit", "exit.png", "Exit from programm",
            TOOL_BAR_WIDTH, TOOL_BAR_HEIGHT, this::jMenuFileExitAction
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
            this.setTitle("Управление загрузкой в EEPROM");

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
            configPanel.setMainFrame(this);

            splitPane.add(configPanel, JSplitPane.RIGHT);
            splitPane.add(mapPanel, JSplitPane.LEFT);
            splitPane.setLastDividerLocation(size.width - 500);
            splitPane.setDividerLocation(size.width - 500);
            splitPane.setResizeWeight(1);
        } catch (final Exception ex) {
            log.error(ex.getMessage(), ex);
        }
    }

    /**
     * File | Exit action performed.
     *
     * @param event Action Event
     */
    public void jMenuFileExitAction(final ActionEvent event) {
        // здесь сохраняем локальную конфигурацию
        System.exit(0);
    }

    /**
     * Help | About action performed.
     *
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

    public void jMenuCreateHexDefinitionAction(final ActionEvent event) {
        final Properties props = Runner.getProperties();
        final JFileChooser chooser = new JFileChooser(props.getProperty(PROP_PATCH));
        final FileNameExtensionFilter filter = new FileNameExtensionFilter(
            "Файл настройки EEPROM или AT24C памяти", "xml", "data");
        chooser.setFileFilter(filter);
        int returnVal = chooser.showOpenDialog(this);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            configPanel.createHexDefinition(chooser.getSelectedFile());
        }
    }

    public void jMenuUploadFileAction(final ActionEvent event) {
        final Properties props = Runner.getProperties();
        final JFileChooser chooser = new JFileChooser(props.getProperty(PROP_PATCH));
        final FileNameExtensionFilter filter = new FileNameExtensionFilter(
            "Дамп загрузки в Arduino", "hex");
        chooser.setFileFilter(filter);
        int returnVal = chooser.showOpenDialog(this);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            configPanel.uploadFile(chooser.getSelectedFile());
        }
    }

    /**
     * Overridden so we can exit when window is closed.
     *
     * @param e оконное событие
     */
    protected void processWindowEvent(final WindowEvent e) {
        super.processWindowEvent(e);
        /* if (e.getID() == WindowEvent.WINDOW_CLOSING) { } */
    }

    /**
     * Вывод сообщений на statusBar.
     *
     * @param out     строка сообщения
     * @param numItem номер элемента статусной строки
     */
    public void outStatus(final String out, final int numItem) {
        jStatusBar.setText(out, numItem);
    }

    /**
     * @param e event which indicates that a component moved
     */
    public void componentResized(final ComponentEvent e) {
        if (e.getComponent().equals(mapPanel)) {
            mapPanel.setRedrawMap(true);
        }
    }

    /**
     * @param e event which indicates that a component moved
     */
    public void componentMoved(final ComponentEvent e) {
    }

    /**
     * @param e event which indicates that a component moved
     */
    public void componentShown(final ComponentEvent e) {
    }

    /**
     * @param e event which indicates that a component moved
     */
    public void componentHidden(final ComponentEvent e) {
    }
}
