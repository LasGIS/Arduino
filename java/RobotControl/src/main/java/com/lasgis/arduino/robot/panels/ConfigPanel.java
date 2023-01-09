/*
 *  @(#)ConfigPanel.java  last: 09.01.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.robot.panels;

import com.fazecast.jSerialComm.SerialPort;
import com.lasgis.serial.PortReader;
import com.lasgis.serial.PortReaderListener;
import com.lasgis.serial.SerialPortWrap;
import lombok.extern.slf4j.Slf4j;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.text.DefaultCaret;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Arrays;
import java.util.Objects;
import java.util.Timer;
import java.util.TimerTask;

import static com.lasgis.util.Util.createImageButton;
import static java.awt.GridBagConstraints.BOTH;
import static java.awt.GridBagConstraints.CENTER;

/**
 * Панель конфигурации.
 *
 * @author VLaskin
 * @version 1.0 Date: 13.01.2005 16:38:05
 */
@Slf4j
public class ConfigPanel extends JPanel implements PortReaderListener {

    private static final Integer[] BAUD_RATES = {
        110, 300, 600, 1200, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 128000, 256000
    };
    private static final Integer[] GEARS = {1, 2, 3, 4, 5};
    private SerialPortWrap[] commPorts = Arrays.stream(SerialPort.getCommPorts()).map(SerialPortWrap::new).toArray(SerialPortWrap[]::new);
    /** Port Reader. */
    private PortReader portReader;
    /** ссылка на MainFrame. */
    private MainFrame mainFrame = null;
    /** дерево конфигурации. */
    private final JPanel controlPanel = new JPanel(new BorderLayout());
    /** панель для информации об ячейках. */
    private final JTextArea arealInfo = new JTextArea();
    /** поле для ввода команды. */
    private final JTextField commandInput = new JTextField();
    /** поле для ввода расстояния. */
    private final JTextField distanceInput = new JTextField();
    /** поле для ввода угла поворота. */
    private final JTextField angleInput = new JTextField();
    /** поле для ввода передачи. */
    private final JComboBox<Integer> gearComboBox = new JComboBox<>(GEARS);
    /** ComboBox for serial ports. */
    private final JComboBox<SerialPortWrap> portsComboBox = new JComboBox<>(commPorts);
    /** ComboBox for baud rates. */
    private final JComboBox<Integer> baudRatesComboBox = new JComboBox<>(BAUD_RATES);

    /** Восстановление или установление связи с девайсом. */
    private final ActionListener resetLinkAction = event -> {
        final JButton button = (JButton) event.getSource();
        final SerialPort serialPort = ((SerialPortWrap) Objects.requireNonNull(portsComboBox.getSelectedItem())).getSerialPort();
        final Integer baudRate = (Integer) baudRatesComboBox.getSelectedItem();
        portReader = PortReader.getPortReader();
        log.debug("Port Reader Action {}", event.getActionCommand());
        if (portReader == null) {
            portReader = PortReader.createPortReader(serialPort, baudRate);
            portReader.addListener(this);
            button.setBackground(new Color(255, 225, 0, 255));
            button.setForeground(new Color(188, 148, 0));
            button.setText("Stop");
        } else if (portReader.getSerialPort() == null) {
            portReader.connect(serialPort, baudRate);
            //portReader.addListener(this);
            button.setBackground(new Color(255, 0, 0, 255));
            button.setForeground(new Color(128, 0, 0));
            button.setText("Stop");
        } else {
            portReader.stop();
            button.setBackground(new Color(0, 255, 0, 89));
            button.setForeground(new Color(0, 128, 0));
            button.setText("Connect");
        }
    };

    /** Обработка события при смене текстовой команды (нажали ENTER в input). */
    private final ActionListener enterOnInputAction = event -> {
        log.debug(event.getActionCommand());
        portReader.writeString(event.getActionCommand());
        commandInput.setText("");
    };

    public void setMainFrame(final MainFrame mainFrame) {
        this.mainFrame = mainFrame;
    }

    enum CommandActionType {AsIs, Move, Turn}

    /** Обработка события нажатия кнопочки. */
    class CommandActionListener implements ActionListener {

        private final String command;
        private final CommandActionType type;

        public CommandActionListener(final String command, final CommandActionType type) {
            this.command = command;
            this.type = type;
        }

        @Override
        public void actionPerformed(final ActionEvent event) {
            final String oldCom = commandInput.getText();
            String outText = oldCom + command + "30" + ";";
            //commandInput.setText(outText);
            portReader.writeString(outText);
            log.debug(outText);
        }
    }

    /**
     * Конструктор.
     */
    public ConfigPanel() {
        super();

        controlPanel.setBackground(MapPanel.PANEL_GRAY_COLOR);
        fillLinkPanel();
        fillNavigationPanel();
        fillParametersPanel();

        /* панель для получения информации от робота. */
        arealInfo.setFont(new Font("Arial", Font.PLAIN, 12));
        final JScrollPane plantInfoScroll = new JScrollPane(arealInfo);
        plantInfoScroll.setViewportView(arealInfo);
        ((DefaultCaret) arealInfo.getCaret()).setUpdatePolicy(DefaultCaret.OUT_BOTTOM);

        /* разделительная панелька */
        final JSplitPane splitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
        splitPane.setContinuousLayout(true);
        splitPane.add(controlPanel, JSplitPane.TOP);
        splitPane.add(plantInfoScroll, JSplitPane.BOTTOM);
        //splitPane.setDividerLocation(100);
        //splitPane.setLastDividerLocation(100);
        splitPane.setResizeWeight(0.0);

        setLayout(new BorderLayout());
        /* панель режима. */
        add(splitPane, BorderLayout.CENTER);
        final Timer tick = new Timer();
        tick.schedule(
            new TimerTask() {
                @Override
                public void run() {
                    final SerialPort[] newCommPorts = SerialPort.getCommPorts();
                    if (commPorts.length != newCommPorts.length) {
                        commPorts = Arrays.stream(newCommPorts).map(SerialPortWrap::new).toArray(SerialPortWrap[]::new);
                        portsComboBox.removeAllItems();
                        for (SerialPortWrap portName : commPorts) {
                            portsComboBox.addItem(portName);
                        }
                    }
                }
            }, 1000, 2000
        );
    }

    /** создание панели связи с COM. */
    private void fillLinkPanel() {
        final JPanel linkPanel = new JPanel();
        linkPanel.setLayout(new BoxLayout(linkPanel, BoxLayout.LINE_AXIS));
        portsComboBox.setSize(16, 20);
        baudRatesComboBox.setSize(16, 20);
        baudRatesComboBox.setSelectedItem(9600);
        final JButton link = createImageButton("Link", null, 80, 20, "Восстановить связь", resetLinkAction);
        link.setBackground(new Color(0, 255, 255, 89));
        link.setForeground(new Color(0, 128, 128));
        link.setBorderPainted(false);
        link.setFocusPainted(false);
        link.setContentAreaFilled(true);
        linkPanel.add(portsComboBox);
        linkPanel.add(baudRatesComboBox);
        linkPanel.add(link);
        controlPanel.add(linkPanel, BorderLayout.NORTH);
    }

    /** создание навигационных кнопок. */
    private void fillNavigationPanel() {
        final JPanel navigationPanel = new JPanel(new GridLayout(3, 4, 5, 5));
        //keyPanel.setSize(150, 150);
        navigationPanel.add(
            createNavigationButton("arrow_up_left.gif", "поворот вперёд и влево", "fl", CommandActionType.Turn)
        );
        navigationPanel.add(
            createNavigationButton("arrow_up.gif", "вперед", "f", CommandActionType.Move)
        );
        navigationPanel.add(
            createNavigationButton("arrow_up_right.gif", "поворот вперёд и вправо", "fr", CommandActionType.Turn)
        );
        navigationPanel.add(
            new JLabel("TL", JLabel.CENTER));
        navigationPanel.add(
            createNavigationButton("arrow_left.gif", "разворот влево на месте", "l", CommandActionType.Turn)
        );
        navigationPanel.add(new JLabel("SC", JLabel.CENTER));
        navigationPanel.add(
            createNavigationButton("arrow_right.gif", "разворот вправо на месте", "r", CommandActionType.Turn)
        );
        navigationPanel.add(new JLabel("TR", JLabel.CENTER));
        navigationPanel.add(
            createNavigationButton("arrow_down_left.gif", "поворот назад и влево", "bl", CommandActionType.Turn)
        );
        navigationPanel.add(
            createNavigationButton("arrow_down.gif", "назад", "b", CommandActionType.Move)
        );
        navigationPanel.add(
            createNavigationButton("arrow_down_right.gif", "поворот назад и вправо", "br", CommandActionType.Turn)
        );
        navigationPanel.add(new JLabel("AUTO", JLabel.CENTER));

        controlPanel.add(navigationPanel, BorderLayout.EAST);

        commandInput.addActionListener(enterOnInputAction);
        controlPanel.add(commandInput, BorderLayout.SOUTH);
    }

    /** создание доп атрибутов. */
    private void fillParametersPanel() {
        distanceInput.setText("30");
        angleInput.setText("90");
        gearComboBox.setSelectedItem(4);
        final JPanel parametersPanel = new JPanel(new GridBagLayout());
        final GridBagConstraints labelGbc = new GridBagConstraints(0, 0, 1, 1, 0, 0,
            CENTER, BOTH, new Insets(2, 5, 2, 4), 0, 0);
        final GridBagConstraints inputGbc = new GridBagConstraints(1, 0, 1, 1, 0, 0,
            CENTER, BOTH, new Insets(2, 0, 2, 0), 0, 0);
        final GridBagConstraints dimensionGbc = new GridBagConstraints(2, 0, 1, 1, 0, 0,
            CENTER, BOTH, new Insets(2, 4, 2, 5), 0, 0);

        parametersPanel.add(new JLabel("Дистанция", JLabel.RIGHT), labelGbc);
        parametersPanel.add(distanceInput, inputGbc);
        parametersPanel.add(new JLabel("[см]", JLabel.LEFT), dimensionGbc);

        labelGbc.gridy++;
        inputGbc.gridy++;
        dimensionGbc.gridy++;
        parametersPanel.add(new JLabel("Угол", JLabel.RIGHT), labelGbc);
        parametersPanel.add(angleInput, inputGbc);
        parametersPanel.add(new JLabel("[град]", JLabel.LEFT), dimensionGbc);

        labelGbc.gridy++;
        inputGbc.gridy++;
        dimensionGbc.gridy++;
        parametersPanel.add(new JLabel("Передача", JLabel.RIGHT), labelGbc);
        parametersPanel.add(gearComboBox, inputGbc);
        parametersPanel.add(new JLabel("gear", JLabel.LEFT), dimensionGbc);

        controlPanel.add(parametersPanel, BorderLayout.WEST);
    }

    private JButton createNavigationButton(
        final String iconName, final String toolTip, final String command, final CommandActionType type
    ) {
        final JButton button = createImageButton(
            null, iconName, 24, 24, toolTip, new CommandActionListener(command, type)
        );
        button.setBorderPainted(true);
        button.setFocusPainted(false);
        button.setContentAreaFilled(true);
        return button;
    }

    public JTextArea getArealInfo() {
        return arealInfo;
    }

    @Override
    public void portReaderCarriageReturn(final String string) {
        arealInfo.append(string);
        arealInfo.append("\n");
    }

    @Override
    public void portReaderTrash(final byte[] data) {
//        arealInfo.append(DatatypeConverter.printHexBinary(data));
//        arealInfo.append("\n");
    }

    @Override
    public void portWriterRun() {

    }
}
