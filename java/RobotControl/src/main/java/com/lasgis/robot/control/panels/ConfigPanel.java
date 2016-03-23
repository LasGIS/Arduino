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
import javax.swing.text.DefaultCaret;
import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Timer;
import java.util.TimerTask;

import com.lasgis.robot.control.serial.PortReader;
import com.lasgis.robot.control.serial.PortReaderListener;
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
public class ConfigPanel extends JPanel implements PortReaderListener {

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
    private String[] portNames = SerialPortList.getPortNames();
    /** ссылка на MainFrame. */
    private MainFrame mainFrame = null;
    /** дерево конфигурации. */
    private final JPanel controlPanel = new JPanel(new BorderLayout());
    /** панель для информации об ячейках. */
    private final JTextArea arealInfo = new JTextArea();
    /** поле для ввода команды. */
    private final JTextField commandInput = new JTextField();
    /** ComboBox for serial ports. */
    private final JComboBox<String> portNamesComboBox = new JComboBox<>(portNames);
    /** ComboBox for baud rates. */
    private final JComboBox<Integer> baudRatesComboBox = new JComboBox<>(BAUD_RATES);

    /** Восстановление или установление связи с девайсом. */
    private final ActionListener resetLinkAction = event -> {
        LOG.debug("Reset Link Action{}", event.getActionCommand());
        final PortReader portReader = PortReader.createPortReader(
            (String) portNamesComboBox.getSelectedItem(), (Integer) baudRatesComboBox.getSelectedItem()
        );
        portReader.addListener(this);
        portReader.addListener(mainFrame.getMapPanel());
    };

    /** Обработка события при смене текстовой команды (нажали ENTER в input). */
    private final ActionListener enterOnInputAction = event -> {
        LOG.debug(event.getActionCommand());
        PortReader.getPortReader().writeString(event.getActionCommand());
        commandInput.setText("");
    };

    public void setMainFrame(final MainFrame mainFrame) {
        this.mainFrame = mainFrame;
    }

    /** Обработка события нажатия кнопочки. */
    class CommandActionListener implements ActionListener {

        private final String command;

        public CommandActionListener(final String command) {
            this.command = command;
        }

        @Override
        public void actionPerformed(final ActionEvent event) {
            final String oldCom = commandInput.getText();
            String outText = oldCom + command + "30" + ";";
            //commandInput.setText(outText);
            PortReader.getPortReader().writeString(outText);
            LOG.debug(outText);
        }
    };

    /**
     * Конструктор.
     */
    public ConfigPanel() {
        super();

        controlPanel.setBackground(MapPanel.PANEL_GRAY_COLOR);
        fillLinkPanel();
        fillNavigationPanel();

        /** панель для получении информации от робота. */
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
                    final String[] newPortNames = SerialPortList.getPortNames();
                    if (portNames.length != newPortNames.length) {
                        portNames = newPortNames;
                        portNamesComboBox.removeAllItems();
                        for (String portName : portNames) {
                            portNamesComboBox.addItem(portName);
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
        portNamesComboBox.setSize(16, 20);
        baudRatesComboBox.setSize(16, 20);
        baudRatesComboBox.setSelectedItem(SerialPort.BAUDRATE_9600);
        final JButton link = createImageButton("Link", null, 64, 20, "Восстановить связь", resetLinkAction);
        link.setBackground(Color.CYAN);
        link.setBorderPainted(false);
        link.setFocusPainted(false);
        link.setContentAreaFilled(true);
        linkPanel.add(portNamesComboBox);
        linkPanel.add(baudRatesComboBox);
        linkPanel.add(link);
        controlPanel.add(linkPanel, BorderLayout.NORTH);
    }

    /** создание навигационных кнопок. */
    private void fillNavigationPanel() {
        final JPanel navigationPanel = new JPanel(new GridLayout(3, 3, 5, 5));
        //keyPanel.setSize(150, 150);
        navigationPanel.add(createNavigationButton("arrow_up_left.gif", "поворот вперёд и влево", "fl"));
        navigationPanel.add(createNavigationButton("arrow_up.gif", "вперед", "f"));
        navigationPanel.add(createNavigationButton("arrow_up_right.gif", "поворот вперёд и вправо", "fr"));
        navigationPanel.add(createNavigationButton("arrow_left.gif", "разворот влево наместе", "l"));
        navigationPanel.add(new JLabel("SC", JLabel.CENTER));
        navigationPanel.add(createNavigationButton("arrow_right.gif", "разворот вправо наместе", "r"));
        navigationPanel.add(createNavigationButton("arrow_down_left.gif", "поворот назад и влево", "bl"));
        navigationPanel.add(createNavigationButton("arrow_down.gif", "назад", "b"));
        navigationPanel.add(createNavigationButton("arrow_down_right.gif", "поворот назад и вправо", "br"));
        controlPanel.add(navigationPanel, BorderLayout.EAST);

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

    @Override
    public void portReaderCarriageReturn(final String string) {

    }

    @Override
    public void portReaderTrash(final String string) {
        arealInfo.append(string);
    }
}
