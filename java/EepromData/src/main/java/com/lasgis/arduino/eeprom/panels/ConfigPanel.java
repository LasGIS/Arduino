/*
 *  @(#)ConfigPanel.java  last: 15.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.panels;

import com.fazecast.jSerialComm.SerialPort;
import com.lasgis.arduino.eeprom.CommandType;
import com.lasgis.arduino.eeprom.Runner;
import com.lasgis.arduino.eeprom.create.CreateHelper;
import com.lasgis.arduino.eeprom.memory.MemoryRoms;
import com.lasgis.arduino.eeprom.test.TestHelper;
import com.lasgis.arduino.eeprom.upload.SerialBlock;
import com.lasgis.arduino.eeprom.upload.UploadHelper;
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
import javax.xml.bind.DatatypeConverter;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.Objects;
import java.util.Timer;
import java.util.TimerTask;

import static com.lasgis.util.Util.createImageButton;
import static java.awt.GridBagConstraints.BOTH;
import static java.awt.GridBagConstraints.CENTER;
import static java.util.Objects.isNull;

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
    private static final DeviceWrap[] DEVICE_WRAPS = {
        DeviceWrap.of(0x00),
        DeviceWrap.of(0x50),
        DeviceWrap.of(0x51),
        DeviceWrap.of(0x52),
        DeviceWrap.of(0x53),
        DeviceWrap.of(0x54),
        DeviceWrap.of(0x55),
        DeviceWrap.of(0x56),
        DeviceWrap.of(0x57)
    };
    /** дерево конфигурации. */
    private final JPanel controlPanel = new JPanel(new BorderLayout());
    /** панель для информации об ячейках. */
    private final JTextArea arealInfo = new JTextArea();
    /** поле для ввода команды. */
    private final JTextField commandInput = new JTextField();
    /** поле для ввода номера микросхемы (0x57 для CMOS). */
    private final JComboBox<DeviceWrap> deviceInput = new JComboBox<>(DEVICE_WRAPS);
    /** поле для ввода адреса блока в EEPROM памяти. */
    private final JTextField addressInput = new JTextField(4);
    /** поле для ввода размера блока. */
    private final JTextField sizeInput = new JTextField(4);
    /** ComboBox for baud rates. */
    private final JComboBox<Integer> baudRatesComboBox = new JComboBox<>(BAUD_RATES);
    private SerialPortWrap[] commPorts = Arrays.stream(SerialPort.getCommPorts()).map(SerialPortWrap::new).toArray(SerialPortWrap[]::new);
    /** ComboBox for serial ports. */
    private final JComboBox<SerialPortWrap> portsComboBox = new JComboBox<>(commPorts);
    /** Port Reader. */
    private PortReader portReader;
    /** Восстановление или установление связи с девайсом. */
    private final ActionListener resetLinkAction = (event -> {
        final JButton button = (JButton) event.getSource();
        final SerialPort serialPort = ((SerialPortWrap) Objects.requireNonNull(portsComboBox.getSelectedItem())).getSerialPort();
        final Integer baudRate = (Integer) baudRatesComboBox.getSelectedItem();
        portReader = PortReader.getPortReader();
        log.debug("Port Reader Action {}", event.getActionCommand());
        if (isNull(portReader)) {
            portReader = PortReader.createPortReader(serialPort, baudRate);
            portReader.addListener(this);
//            portReader.addListener(mainFrame.getMapPanel());
            button.setBackground(new Color(255, 225, 0));
            button.setForeground(new Color(188, 148, 0));
            button.setText("Stop");
        } else if (isNull(portReader.getSerialPort())) {
            portReader.connect(serialPort, baudRate);
            button.setBackground(new Color(255, 0, 0));
            button.setForeground(new Color(128, 0, 0));
            button.setText("Stop");
        } else {
            portReader.stop();
            button.setBackground(new Color(0, 255, 0));
            button.setForeground(new Color(0, 128, 0));
            button.setText("Connect");
        }
    });
    /** ссылка на MainFrame. */
    private MainFrame mainFrame = null;

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

    /** Обработка события при смене текстовой команды (нажали ENTER в input). */
    private final ActionListener enterOnInputAction = event -> {
        final String command = event.getActionCommand();
        log.debug(command);
        portReader.writeString(command);
        commandInput.setText("");
        arealInfo.append(">> " + command + "\n");
    };

    public void setMainFrame(final MainFrame mainFrame) {
        this.mainFrame = mainFrame;
    }

    /** создание панели связи с COM. */
    private void fillLinkPanel() {
        final JPanel linkPanel = new JPanel();
        linkPanel.setLayout(new BoxLayout(linkPanel, BoxLayout.LINE_AXIS));
        portsComboBox.setSize(16, 20);
        baudRatesComboBox.setSize(16, 20);
        baudRatesComboBox.setSelectedItem(9600);
        final JButton link = createImageButton("Link", null, 80, 20, "Восстановить связь", resetLinkAction);
        link.setBackground(new Color(0, 255, 255));
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
        final JPanel navigationPanel = new JPanel(new GridLayout(3, 2, 5, 5));
        //keyPanel.setSize(150, 150);
        navigationPanel.add(createNavigationButton("Create", null, "Создаем по файлу", CommandType.create));
        navigationPanel.add(createNavigationButton("Upload", null, "Запись блока", CommandType.upload));
        navigationPanel.add(createNavigationButton("Read", null, "Чтение блока", CommandType.read));
        controlPanel.add(navigationPanel, BorderLayout.EAST);

        commandInput.addActionListener(enterOnInputAction);
        controlPanel.add(commandInput, BorderLayout.SOUTH);
    }

    /** создание доп атрибутов. */
    private void fillParametersPanel() {
        deviceInput.setSelectedIndex(1);
        addressInput.setText("0000");
        sizeInput.setText("0200");
        final JPanel parametersPanel = new JPanel(new GridBagLayout());
        final GridBagConstraints labelGbc = new GridBagConstraints(0, 0, 1, 1, 0, 0,
            CENTER, BOTH, new Insets(2, 5, 2, 4), 0, 0);
        final GridBagConstraints inputGbc = new GridBagConstraints(1, 0, 1, 1, 0, 0,
            CENTER, BOTH, new Insets(2, 0, 2, 0), 0, 0);

        parametersPanel.add(new JLabel("device", JLabel.RIGHT), labelGbc);
        parametersPanel.add(deviceInput, inputGbc);

        labelGbc.gridy++;
        inputGbc.gridy++;
        parametersPanel.add(new JLabel("address", JLabel.RIGHT), labelGbc);
        parametersPanel.add(addressInput, inputGbc);

        labelGbc.gridy++;
        inputGbc.gridy++;
        parametersPanel.add(new JLabel("size", JLabel.RIGHT), labelGbc);
        parametersPanel.add(sizeInput, inputGbc);

        controlPanel.add(parametersPanel, BorderLayout.WEST);
    }

    private JButton createNavigationButton(final String name, final String iconName, final String toolTip, final CommandType command) {
        final JButton button = createImageButton(
            name, iconName, 100, 24, toolTip, new CommandActionListener(command)
        );
        button.setBorderPainted(true);
        button.setFocusPainted(true);
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

    enum CommandActionType {AsIs, Move, Turn}

    /** Обработка события нажатия кнопочки. */
    class CommandActionListener implements ActionListener {

        private final CommandType command;

        public CommandActionListener(final CommandType command) {
            this.command = command;
        }

        @Override
        public void actionPerformed(final ActionEvent event) {
            final MemoryRoms memoryRoms = Runner.getMemoryRoms();
            switch (command) {
                case test: {
                    TestHelper.show(memoryRoms);
                    break;
                }
                case create: {
                    try {
                        CreateHelper.create(memoryRoms);
                    } catch (IOException ex) {
                        log.error(ex.getMessage(), ex);
                    }
                    break;
                }
                case upload: {
                    try {
                        final byte device = ((DeviceWrap) deviceInput.getSelectedItem()).getDevice();
                        UploadHelper.uploadFile(device, portReader);
                    } catch (InterruptedException | IOException ex) {
                        log.error(ex.getMessage(), ex);
                    }
                    break;
                }
                case read: {
                    try {
                        final ByteBuffer buffer = ByteBuffer.allocate(10);
                        final byte device = ((DeviceWrap) deviceInput.getSelectedItem()).getDevice();
                        buffer.put(DatatypeConverter.parseHexBinary(addressInput.getText()));
                        buffer.put(DatatypeConverter.parseHexBinary(sizeInput.getText()));
                        short address = buffer.getShort(0);
                        short size = buffer.getShort(2);
                        UploadHelper.readBlock(portReader, SerialBlock.of(
                            device, address, size, (short) 0
                        ));
                    } catch (InterruptedException ex) {
                        log.error(ex.getMessage(), ex);
                    }
                    break;
                }
            }
        }
    }
}
