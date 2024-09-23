/*
 *  @(#)ConfigPanel.java  last: 24.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.panels;

import com.lasgis.arduino.editfont.CommandType;
import com.lasgis.arduino.editfont.data.FontDataAdapter;
import com.lasgis.arduino.editfont.data.FontDataListener;
import com.lasgis.arduino.editfont.data.FontDataPerformed;
import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

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
public class ConfigPanel extends JPanel {

//    private final FontData fontData;

    /** Дерево конфигурации. */
    private final JPanel controlPanel = new JPanel(new BorderLayout());
    /** Панель для информации об ячейках. */
    @Getter
    private final JTextArea arealInfo = new JTextArea("0x00, 0x00, 0x00, 0x61, 0x91, 0x92, 0x64, 0x08, 0x10, 0x26, 0x49, 0x89, 0x86, 0x00, 0x00, 0x00");
    private final JFileChooserField cppFileInput;
    private final JFileChooserField hppFileInput;
    private final JTextField numberCharsInput = new JTextField(8);
    private final JTextField charHeightInput = new JTextField(8);
    private final JTextField baseLineInput = new JTextField(8);
    private final JTextField firstCharInput = new JTextField(8);
    private final JTextField widthCharInput = new JTextField(8);

    final FontDataListener fontDataListener = new FontDataAdapter() {

        @Override
        public boolean onChangeCFile(File file) {
            cppFileInput.setFileName(file.getAbsolutePath());
            return true;
        }

        @Override
        public boolean onChangeHFile(File file) {
            hppFileInput.setFileName(file.getAbsolutePath());
            return true;
        }

        @Override
        public boolean onChangeWidthChar(Integer value) {
            widthCharInput.setText(value.toString());
            return true;
        }

        @Override
        public boolean onChangeNumberChars(Integer value) {
            numberCharsInput.setText(value.toString());
            return true;
        }

        @Override
        public boolean onChangeCharHeight(Integer value) {
            charHeightInput.setText(value.toString());
            return true;
        }

        @Override
        public boolean onChangeBaseLine(Integer value) {
            baseLineInput.setText(value.toString());
            return true;
        }

        @Override
        public boolean onChangeFirstChar(Integer value) {
            firstCharInput.setText(value.toString());
            return true;
        }
    };

    /** Конструктор. */
    public ConfigPanel() {
        super();
        cppFileInput = new JFileChooserField(
            new FileNameExtensionFilter("Файл CPP, содержащий шрифт", "c", "cpp"),
            (file) -> {
                log.debug("Изменили CPP File \"{}\"", file.getAbsolutePath());
                FontDataPerformed.setCFile(file);
            }
        );
        hppFileInput = new JFileChooserField(
            new FileNameExtensionFilter("Файл HPP, содержащий описание шрифта", "h", "hpp"),
            (file) -> {
                log.debug("Изменили H File \"{}\"", file.getAbsolutePath());
                FontDataPerformed.setHFile(file);
            }
        );

        controlPanel.setBackground(MapPanel.PANEL_GRAY_COLOR);
        fillParametersPanel();

        /* панель для получения информации */
        arealInfo.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 12));
        arealInfo.setLineWrap(true);
        arealInfo.setWrapStyleWord(true);
        final JScrollPane plantInfoScroll = new JScrollPane(arealInfo);
        plantInfoScroll.setViewportView(arealInfo);

        /* разделительная панелька */
        final JSplitPane splitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
        splitPane.setContinuousLayout(true);
        splitPane.add(controlPanel, JSplitPane.TOP);
        splitPane.add(plantInfoScroll, JSplitPane.BOTTOM);
        splitPane.setResizeWeight(0.0);

        setLayout(new BorderLayout());
        /* панель режима. */
        add(splitPane, BorderLayout.CENTER);
    }

    /** Создание доп атрибутов. */
    private void fillParametersPanel() {
        final JPanel parametersPanel = new JPanel(new GridBagLayout());
        final GridBagConstraints gbc = new GridBagConstraints(0, 0, 1, 1, 0, 0,
            CENTER, BOTH, new Insets(5, 5, 5, 5), 0, 0);

        gbc.gridy = 0;
        gbc.gridx = 0;
        parametersPanel.add(new JLabel("Файл шрифта .c", JLabel.RIGHT), gbc);
        gbc.gridx = 1;
        gbc.weightx = 1.0;
        parametersPanel.add(cppFileInput, gbc);

        gbc.gridy = 1;
        gbc.gridx = 0;
        parametersPanel.add(new JLabel("Файл Описания .h", JLabel.RIGHT), gbc);
        gbc.gridx = 1;
        gbc.weightx = 1.0;
        parametersPanel.add(hppFileInput, gbc);

        gbc.gridy = 2;
        gbc.gridx = 0;
        parametersPanel.add(new JLabel("Число знаков", JLabel.RIGHT), gbc);
        gbc.gridx = 1;
        parametersPanel.add(numberCharsInput, gbc);

        gbc.gridy = 3;
        gbc.gridx = 0;
        parametersPanel.add(new JLabel("Высота знаков", JLabel.RIGHT), gbc);
        gbc.gridx = 1;
        parametersPanel.add(charHeightInput, gbc);

        gbc.gridy = 4;
        gbc.gridx = 0;
        parametersPanel.add(new JLabel("Средняя линия", JLabel.RIGHT), gbc);
        gbc.gridx = 1;
        parametersPanel.add(baseLineInput, gbc);

        gbc.gridy = 5;
        gbc.gridx = 0;
        parametersPanel.add(new JLabel("Ширина знака", JLabel.RIGHT), gbc);
        gbc.gridx = 1;
        parametersPanel.add(widthCharInput, gbc);

        final JPanel buttonPanel = new JPanel(new GridBagLayout());
        gbc.gridy = 0;
        gbc.gridx = 0;
        gbc.gridwidth = 1;
        gbc.weightx = 0.0;
        buttonPanel.add(createNavigationButton("Create", "create.png", "Создаем по файлу", CommandType.create), gbc);
        gbc.gridx = 1;
        buttonPanel.add(createNavigationButton("Upload", "upload.png", "Запись блока", CommandType.upload), gbc);
        gbc.gridx = 2;
        buttonPanel.add(createNavigationButton("Read", "userGuide.png", "Чтение блока", CommandType.read), gbc);

        controlPanel.add(parametersPanel, BorderLayout.CENTER);
        controlPanel.add(buttonPanel, BorderLayout.SOUTH);
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

    /** Обработка события нажатия кнопочки. */
    static class CommandActionListener implements ActionListener {

        private final CommandType command;

        public CommandActionListener(final CommandType command) {
            this.command = command;
        }

        @Override
        public void actionPerformed(final ActionEvent event) {
            switch (command) {
                case create: {
                    try {
                        log.info("create");
                    } catch (Exception ex) {
                        log.error(ex.getMessage(), ex);
                    }
                    break;
                }
                case upload: {
                    try {
                        log.info("upload");
                    } catch (Exception ex) {
                        log.error(ex.getMessage(), ex);
                    }
                    break;
                }
                case read: {
                    try {
                        log.info("read");
                    } catch (Exception ex) {
                        log.error(ex.getMessage(), ex);
                    }
                    break;
                }
            }
        }
    }
}
