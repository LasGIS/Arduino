/*
 *  @(#)JFileChooserField.java  last: 22.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.panels;

import com.lasgis.util.Util;
import lombok.extern.slf4j.Slf4j;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.EventListener;

/**
 * <description>
 *
 * @author VLaskin
 * @since <pre>06.03.2023</pre>
 */
@Slf4j
public class JFileChooserField extends JPanel implements ActionListener {
    private final JTextField filename = new JTextField();
    private final FileNameExtensionFilter filter;
    private final FileChooserListener onChangeListener;

    interface FileChooserListener extends EventListener {
        void actionPerformed(final File file);
    }

    public JFileChooserField(
        final FileNameExtensionFilter filter,
        final FileChooserListener onChangeListener
    ) {
        super();
        setLayout(new BoxLayout(this, BoxLayout.LINE_AXIS));
        this.filter = filter;
        this.onChangeListener = onChangeListener;
        final JButton button = new JButton();
        final Dimension dimension = new Dimension(24, 24);
        try {
            button.setIcon(Util.loadImageIcon("arrow_up.gif"));
        } catch (final IOException e) {
            log.error(e.getMessage());
        }
        button.setMaximumSize(dimension);
        button.setMinimumSize(dimension);
        button.setPreferredSize(dimension);
        button.addActionListener(this);
        add(filename);
        add(button);
    }

    public void setFileName(final String fileName) {
        this.filename.setText(fileName);
    }

    @Override
    public void actionPerformed(final ActionEvent e) {
        final JFileChooser chooser = new JFileChooser(filename.getText());
        chooser.setFileFilter(filter);
        int returnVal = chooser.showOpenDialog(this);
        if (returnVal == JFileChooser.APPROVE_OPTION) {
            final File file = chooser.getSelectedFile();
            filename.setText(file.getAbsolutePath());
            onChangeListener.actionPerformed(file);
        }
    }

    public File getFile() throws FileNotFoundException {
        final File file = new File(filename.getText());
        if (file.isFile()) {
            return file;
        } else {
            JOptionPane.showMessageDialog(null, "Файл не найден!", "Error", JOptionPane.ERROR_MESSAGE);
            throw new FileNotFoundException(filename.getText());
        }
    }
}
