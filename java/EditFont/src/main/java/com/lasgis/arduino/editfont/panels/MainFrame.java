/*
 *  @(#)MainFrame.java  last: 24.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.panels;

import com.lasgis.arduino.editfont.data.FontDataAdapter;
import com.lasgis.arduino.editfont.data.FontDataListener;
import com.lasgis.arduino.editfont.data.FontDataPerformed;
import com.lasgis.component.StatusBar;
import com.lasgis.util.SettingMenuItem;
import com.lasgis.util.SettingToolBarItem;
import com.lasgis.util.Util;
import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTabbedPane;
import javax.swing.JTextArea;
import javax.swing.JToolBar;
import java.awt.AWTEvent;
import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

/**
 * MainFrame class
 *
 * @author VLaskin
 * @version 1.0
 */
@Slf4j
public class MainFrame extends JFrame implements ComponentListener {

    /** Ширина кнопки на главной панели инструментов. */
    private static final int TOOL_BAR_WIDTH = 27;
    /** Высота кнопки на главной панели инструментов. */
    private static final int TOOL_BAR_HEIGHT = 27;
    private static final int CONFIG_PANEL_WIDTH = 370;

    /** Строка состояния. */
    private final StatusBar jStatusBar = new StatusBar(new int[]{0, 100, 200});
    @Getter
    private final JTextArea cFileArea = new JTextArea();
    @Getter
    private final JTextArea hFileArea = new JTextArea();
    /** Панель с картой. */
    @Getter
    private final MapPanel mapPanel;
    /** Панель конфигурации. */
    @Getter
    private final ConfigPanel configPanel;

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
            "Помощь", "userGuide.png", "Help",
            TOOL_BAR_WIDTH, TOOL_BAR_HEIGHT, this::jMenuHelpAboutAction
        ),
        new SettingToolBarItem(
            "Exit", "exit.png", "Exit from programm",
            TOOL_BAR_WIDTH, TOOL_BAR_HEIGHT, this::jMenuFileExitAction
        )
    };

    final FontDataListener fontDataListener = new FontDataAdapter() {
        @Override
        public boolean onChangeCSource(StringBuilder stringBuilder) {
            cFileArea.setText(stringBuilder.toString());
            return true;
        }

        @Override
        public boolean onChangeHSource(StringBuilder stringBuilder) {
            hFileArea.setText(stringBuilder.toString());
            return true;
        }
    };

    /**
     * Construct the frame.
     */
    public MainFrame() {
        mapPanel = new MapPanel(this);
        FontDataPerformed.addListener(mapPanel.fontDataListener, true);
        configPanel = new ConfigPanel();
        FontDataPerformed.addListener(configPanel.fontDataListener, true);

        enableEvents(AWTEvent.WINDOW_EVENT_MASK);
//        final FontData fontData = FontDataPerformed.getFontData();
        try {
            final Dimension size = new Dimension(1450, 800);
            final JPanel contentPane = (JPanel) this.getContentPane();
            contentPane.setLayout(new BorderLayout());
            this.setSize(size);
            this.setTitle("Редактор Шрифтов");

            /* Настраиваем главное меню */
            final JMenuBar menuBar = new JMenuBar();
            for (SettingMenuItem aSetMenu : menuSetting) {
                menuBar.add(Util.createImageMenuItem(aSetMenu));
            }

            /* Настраиваем главный ToolBar */
            final JToolBar toolBar = new JToolBar();
            for (SettingToolBarItem aSetToolBar : toolBarSetting) {
                toolBar.add(Util.createImageButton(aSetToolBar));
            }

            /* Разделительная панелька */
            final JSplitPane splitPane = new JSplitPane();
            splitPane.setContinuousLayout(true);

            final Font monoFont = new Font(Font.MONOSPACED, Font.PLAIN, 12);
            /* Показываем C File */
            cFileArea.setFont(monoFont);
            final JScrollPane cFileScroll = new JScrollPane(cFileArea);
            cFileScroll.setViewportView(cFileArea);

            /* Показываем H File */
            hFileArea.setFont(monoFont);
            final JScrollPane hFileScroll = new JScrollPane(hFileArea);
            hFileScroll.setViewportView(hFileArea);

            /* Панель вкладок */
            final JTabbedPane tabbedPane = new JTabbedPane(JTabbedPane.TOP, JTabbedPane.WRAP_TAB_LAYOUT);
            tabbedPane.addTab("C Font File", cFileScroll);
            tabbedPane.addTab("H Font File", hFileScroll);
            tabbedPane.addTab("Редактор Знака", mapPanel);
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

            splitPane.add(configPanel, JSplitPane.RIGHT);
            splitPane.add(tabbedPane, JSplitPane.LEFT);
            splitPane.setLastDividerLocation(size.width - CONFIG_PANEL_WIDTH);
            splitPane.setDividerLocation(size.width - CONFIG_PANEL_WIDTH);
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
