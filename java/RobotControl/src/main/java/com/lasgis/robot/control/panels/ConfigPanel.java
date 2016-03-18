/**
 * @(#)ConfigPanel.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2015 LasGIS Company. All Rights Reserved.
 */

package com.lasgis.robot.control.panels;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import jssc.SerialPort;
import jssc.SerialPortList;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import static com.lasgis.util.Util.createImageButton;

/**
 * Панель конфигурации.
 * @author VLaskin
 * @version 1.0 Date: 13.01.2005 16:38:05
 */
public class ConfigPanel extends JPanel {

    private static final Logger LOG = LoggerFactory.getLogger(ConfigPanel.class);

    private static final Integer[] BAUD_RATES = {
        SerialPort.BAUDRATE_110,
        SerialPort.BAUDRATE_300,
        SerialPort.BAUDRATE_600,
        SerialPort.BAUDRATE_1200,
        SerialPort.BAUDRATE_4800,
        SerialPort.BAUDRATE_9600,
        SerialPort.BAUDRATE_14400,
        SerialPort.BAUDRATE_19200,
        SerialPort.BAUDRATE_38400,
        SerialPort.BAUDRATE_57600,
        SerialPort.BAUDRATE_115200,
        SerialPort.BAUDRATE_128000,
        SerialPort.BAUDRATE_256000,
    };
    /** дерево конфигурации. */
    private final JPanel controlPanel = new JPanel(new BorderLayout());
    /** панель для информации об ячейках. */
    private final JTextArea arealInfo = new JTextArea();
    /** поле для ввода команды. */
    private final JTextField commandInput = new JTextField();

    /** Обработка события при смене текстовой команды (нажали ENTER в input). */
    private final ActionListener enterOnInputAction = event -> {
        LOG.debug(event.getActionCommand());
        commandInput.setText("");
    };

    /** Обработка события нажатия кнопочки. */
    class CommandActionListener implements ActionListener {

        private final String command;

        public CommandActionListener(final String command) {
            this.command = command;
        }

        @Override
        public void actionPerformed(final ActionEvent event) {
            final String oldCom = commandInput.getText();
            commandInput.setText(oldCom + command + "30" + ";");
        }
    };

    /**
     * Конструктор.
     */
    public ConfigPanel() {
        super();

        fillControlPanel();

        /** панель для получении информации от робота. */
        arealInfo.setFont(new Font("Arial", Font.PLAIN, 12));
        final JScrollPane plantInfoScroll = new JScrollPane(arealInfo);
        plantInfoScroll.setViewportView(arealInfo);

        /* разделительная панелька */
        final JSplitPane splitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
        splitPane.setContinuousLayout(true);
        splitPane.add(controlPanel, JSplitPane.TOP);
        splitPane.add(plantInfoScroll, JSplitPane.BOTTOM);
        splitPane.setDividerLocation(100);
        splitPane.setLastDividerLocation(100);
        splitPane.setResizeWeight(0.0);

        setLayout(new BorderLayout());
        /* панель режима. */
        add(splitPane, BorderLayout.CENTER);

    }

    /**
     * заполняем кнпочки
     */
    private void fillControlPanel() {
        controlPanel.setBackground(MapPanel.PANEL_GRAY_COLOR);

        /* создание панели связи с COM */
        final JPanel comPanel = new JPanel();
        comPanel.setLayout(new BoxLayout(comPanel, BoxLayout.LINE_AXIS));
        final JComboBox<String> coms = new JComboBox<>(SerialPortList.getPortNames());
        coms.setSize(16, 20);
        final JComboBox<Integer> rates = new JComboBox<>(BAUD_RATES);
        rates.setSize(16, 20);
        rates.setSelectedItem(SerialPort.BAUDRATE_9600);
        final JButton link = createImageButton("Link", null, 64, 20, "Восстановить связь", null);
        link.setBackground(Color.CYAN);
        link.setBorderPainted(false);
        link.setFocusPainted(false);
        link.setContentAreaFilled(true);
        comPanel.add(coms);
        comPanel.add(rates);
        comPanel.add(link);
        controlPanel.add(comPanel, BorderLayout.NORTH);

        /* создание навигационных кнопок */
        final JPanel keyPanel = new JPanel(new GridLayout(3, 3, 5, 5));
        keyPanel.setSize(100, 100);
        keyPanel.add(createNavigationButton("arrow_up_left.gif", "поворот вперёд и влево", "fl"));
        keyPanel.add(createNavigationButton("arrow_up.gif", "вперед", "f"));
        keyPanel.add(createNavigationButton("arrow_up_right.gif", "поворот вперёд и вправо", "fr"));
        keyPanel.add(createNavigationButton("arrow_left.gif", "разворот влево наместе", "l"));
        keyPanel.add(new JLabel("SC", JLabel.CENTER));
        keyPanel.add(createNavigationButton("arrow_right.gif", "разворот вправо наместе", "r"));
        keyPanel.add(createNavigationButton("arrow_down_left.gif", "поворот назад и влево", "bl"));
        keyPanel.add(createNavigationButton("arrow_down.gif", "назад", "b"));
        keyPanel.add(createNavigationButton("arrow_down_right.gif", "поворот назад и вправо", "br"));
        controlPanel.add(keyPanel, BorderLayout.EAST);

        commandInput.addActionListener(enterOnInputAction);
        controlPanel.add(commandInput, BorderLayout.SOUTH);
    }

    private JButton createNavigationButton(
        final String iconName, final String toolTip, final String command
    ) {
        final JButton button = createImageButton(null, iconName, 24, 24, toolTip, new CommandActionListener(command));
        button.setBorderPainted(false);
        button.setFocusPainted(false);
        button.setContentAreaFilled(true);
        return button;
    }

    public JTextArea getArealInfo() {
        return arealInfo;
    }
}
