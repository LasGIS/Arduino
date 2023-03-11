/*
 *  @(#)ConfigPanel.java  last: 11.03.2023
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
import com.lasgis.arduino.eeprom.load.LoadHelper;
import com.lasgis.arduino.eeprom.memory.MemoryRoms;
import com.lasgis.arduino.eeprom.upload.SerialBlock;
import com.lasgis.arduino.eeprom.upload.UploadHelper;
import com.lasgis.serial.PortReader;
import com.lasgis.serial.PortReaderListener;
import com.lasgis.serial.SerialPortWrap;
import lombok.extern.slf4j.Slf4j;
import org.apache.commons.io.FilenameUtils;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.swing.text.DefaultCaret;
import javax.xml.bind.DatatypeConverter;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.Objects;
import java.util.Optional;
import java.util.Properties;
import java.util.Timer;
import java.util.TimerTask;

import static com.lasgis.arduino.eeprom.Runner.PROP_DATA_FILE;
import static com.lasgis.arduino.eeprom.Runner.PROP_PATCH;
import static com.lasgis.util.Util.createImageButton;
import static java.awt.GridBagConstraints.BOTH;
import static java.awt.GridBagConstraints.CENTER;
import static java.util.Objects.isNull;
import static java.util.Objects.nonNull;

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
    private final JFileChooserField dataFileInput;
    private final JFileChooserField hexFileInput;
    /** поле для ввода номера микросхемы (0x57 для CMOS). */
    private final JComboBox<DeviceWrap> deviceInput = new JComboBox<>(DEVICE_WRAPS);
    /** поле для ввода адреса блока в EEPROM памяти. */
    private final JTextField addressInput = new JTextField(8);
    /** поле для ввода размера блока. */
    private final JTextField sizeInput = new JTextField(8);
    /** ComboBox for baud rates. */
    private final JComboBox<Integer> baudRatesComboBox = new JComboBox<>(BAUD_RATES);
    /** Button Link */
    private final JButton link = createImageButton("Link", null, 80, 20, "Восстановить связь", null);
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
    /** Обработка события при смене текстовой команды (нажали ENTER в input). */
    private final ActionListener enterOnInputAction = event -> {
        final String command = event.getActionCommand();
        log.debug(command);
        portReader.writeString(command);
        commandInput.setText("");
        arealInfo.append(">> " + command + "\n");
    };
    /** ссылка на MainFrame. */
    private MainFrame mainFrame = null;

    /**
     * Конструктор.
     */
    public ConfigPanel() {
        super();
        final Properties props = Runner.getProperties();
        dataFileInput = new JFileChooserField(
            new File(props.getProperty(PROP_PATCH), props.getProperty(PROP_DATA_FILE)).getPath(),
            new FileNameExtensionFilter("Файл настройки EEPROM или AT24C памяти", "xml", "data")
        );
        String headerFilename = null;
        try {
            final MemoryRoms memoryRoms = LoadHelper.load();
            headerFilename = FilenameUtils.removeExtension(memoryRoms.getHeaderFilename());
        } catch (Exception ex) {
            log.error(ex.getMessage(), ex);
        }
        hexFileInput = new JFileChooserField(
            nonNull(headerFilename) ? Path.of(props.getProperty(PROP_PATCH), headerFilename + ".hex").toString() : props.getProperty(PROP_PATCH),
            new FileNameExtensionFilter("Дамп загрузки в Arduino", "hex")
        );
        final Dimension dimension = new Dimension(70, 20);
        deviceInput.setMinimumSize(dimension);
        deviceInput.setPreferredSize(dimension);
        addressInput.setMinimumSize(dimension);
        addressInput.setMaximumSize(dimension);
        sizeInput.setMinimumSize(dimension);
        sizeInput.setMaximumSize(dimension);

        controlPanel.setBackground(MapPanel.PANEL_GRAY_COLOR);
        fillLinkPanel();
        fillParametersPanel();

        /* панель для получения информации */
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

    private void checkPortReader() {
        final SerialPort serialPort = ((SerialPortWrap) Objects.requireNonNull(portsComboBox.getSelectedItem())).getSerialPort();
        final int baudRate = (Integer) Optional.ofNullable(baudRatesComboBox.getSelectedItem()).orElse(9600);
        portReader = PortReader.getPortReader();
        log.debug("check Port Reader");
        if (isNull(portReader)) {
            portReader = PortReader.createPortReader(serialPort, baudRate);
            portReader.addListener(this);
//            portReader.addListener(mainFrame.getMapPanel());
            link.setBackground(new Color(255, 225, 0));
            link.setForeground(new Color(188, 148, 0));
            link.setText("Stop");
        } else if (isNull(portReader.getSerialPort())) {
            portReader.connect(serialPort, baudRate);
            link.setBackground(new Color(255, 0, 0));
            link.setForeground(new Color(128, 0, 0));
            link.setText("Stop");
        }
    }

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
        link.addActionListener(resetLinkAction);
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

    /** создание доп атрибутов. */
    private void fillParametersPanel() {
        deviceInput.setSelectedIndex(1);
        addressInput.setText("0000");
        sizeInput.setText("0200");
        final JPanel parametersPanel = new JPanel(new GridBagLayout());
        final GridBagConstraints gbc = new GridBagConstraints(0, 0, 1, 1, 0, 0,
            CENTER, BOTH, new Insets(5, 5, 5, 5), 0, 0);

        gbc.gridy = 0;
        gbc.gridx = 0;
        parametersPanel.add(new JLabel("data File", JLabel.RIGHT), gbc);
        gbc.gridx = 1;
        gbc.gridwidth = 6;
        gbc.weightx = 1.0;
        parametersPanel.add(dataFileInput, gbc);
        gbc.gridx = 7;
        gbc.gridwidth = 1;
        gbc.weightx = 0.0;
        parametersPanel.add(createNavigationButton("Create", "create.png", "Создаем по файлу", CommandType.create), gbc);

        gbc.gridy = 1;
        gbc.gridx = 0;
        parametersPanel.add(new JLabel("HEX File", JLabel.RIGHT), gbc);
        gbc.gridx = 1;
        gbc.gridwidth = 6;
        gbc.weightx = 1.0;
        parametersPanel.add(hexFileInput, gbc);
        gbc.gridx = 7;
        gbc.gridwidth = 1;
        gbc.weightx = 0.0;
        parametersPanel.add(createNavigationButton("Upload", "upload.png", "Запись блока", CommandType.upload), gbc);

        gbc.gridy = 2;
        gbc.gridx = 0;
        parametersPanel.add(new JLabel("device", JLabel.RIGHT), gbc);
        gbc.gridx = 1;
        parametersPanel.add(deviceInput, gbc);
        gbc.gridx = 2;
        parametersPanel.add(new JLabel("address", JLabel.RIGHT), gbc);
        gbc.gridx = 3;
//        gbc.weightx = 0.1;
        parametersPanel.add(addressInput, gbc);
        gbc.gridx = 4;
//        gbc.weightx = 0.0;
        parametersPanel.add(new JLabel("size", JLabel.RIGHT), gbc);
        gbc.gridx = 5;
//        gbc.weightx = 0.1;
        parametersPanel.add(sizeInput, gbc);
        gbc.gridx = 7;
//        gbc.weightx = 0.0;
        parametersPanel.add(createNavigationButton("Read", "userGuide.png", "Чтение блока", CommandType.read), gbc);

        controlPanel.add(parametersPanel, BorderLayout.CENTER);

        commandInput.addActionListener(enterOnInputAction);
        controlPanel.add(commandInput, BorderLayout.SOUTH);
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

    public void createHexDefinition(final File file) {
        try {
            final MemoryRoms memoryRoms = LoadHelper.load(file);
            final String path = file.getParent();
            CreateHelper.create(nonNull(path) ? path : "", memoryRoms);
        } catch (Exception ex) {
            log.error(ex.getMessage(), ex);
        }
    }

    public void uploadFile(final File file) {
        try {
            checkPortReader();
            UploadHelper.uploadFile(portReader, file);
        } catch (Exception ex) {
            log.error(ex.getMessage(), ex);
        }
    }

    /** Обработка события нажатия кнопочки. */
    class CommandActionListener implements ActionListener {

        private final CommandType command;

        public CommandActionListener(final CommandType command) {
            this.command = command;
        }

        @Override
        public void actionPerformed(final ActionEvent event) {
            checkPortReader();
            switch (command) {
                case create: {
                    try {
                        final File file = dataFileInput.getFile();
                        final MemoryRoms memoryRoms = LoadHelper.load(file);
                        final String path = file.getParent();
                        CreateHelper.create(nonNull(path) ? path : "", memoryRoms);
                    } catch (Exception ex) {
                        log.error(ex.getMessage(), ex);
                    }
                    break;
                }
                case upload: {
                    try {
                        final File file = hexFileInput.getFile();
                        UploadHelper.uploadFile(portReader, file);
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
