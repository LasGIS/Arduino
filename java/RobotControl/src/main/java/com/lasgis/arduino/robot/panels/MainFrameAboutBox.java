/*
 *  @(#)MainFrameAboutBox.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.robot.panels;

import com.lasgis.util.Util;
import lombok.extern.slf4j.Slf4j;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;

/**
 * Created by IntelliJ IDEA.
 * @author VLaskin
 * @version 1.0
 */
@Slf4j
public class MainFrameAboutBox extends JDialog implements ActionListener {

    private JButton button1 = new JButton();

    /**
     * Create and show dialog.
     * @param parent parent frame
     */
    public MainFrameAboutBox(final Frame parent) {
        super(parent);
        enableEvents(AWTEvent.WINDOW_EVENT_MASK);
        try {
            final JLabel imageLabel = new JLabel();
            imageLabel.setIcon(Util.loadImageIcon("Evolution.GIF"));
            this.setTitle("About");
            final JPanel panel1 = new JPanel();
            panel1.setLayout(new BorderLayout());
            final JPanel panel2 = new JPanel();
            panel2.setLayout(new BorderLayout());
            final JPanel insetsPanel1 = new JPanel();
            final FlowLayout flowLayout1 = new FlowLayout();
            insetsPanel1.setLayout(flowLayout1);
            final JPanel insetsPanel2 = new JPanel();
            insetsPanel2.setLayout(flowLayout1);
            insetsPanel2.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
            final GridLayout gridLayout1 = new GridLayout();
            gridLayout1.setRows(4);
            gridLayout1.setColumns(1);
            final JLabel label1 = new JLabel();
            final String product = "Hobby ROBOT support";
            label1.setText(product);
            final JLabel label2 = new JLabel();
            final String version = "1.0";
            label2.setText(version);
            final JLabel label3 = new JLabel();
            final String copyright = "Copyright (c) 2016";
            label3.setText(copyright);
            final JLabel label4 = new JLabel();
            final String comments = "Control for ROBOT throws Bluetooth";
            label4.setText(comments);
            final JPanel insetsPanel3 = new JPanel();
            insetsPanel3.setLayout(gridLayout1);
            insetsPanel3.setBorder(BorderFactory.createEmptyBorder(10, 60, 10, 10));
            button1.setText("Ok");
            button1.addActionListener(this);
            insetsPanel2.add(imageLabel, null);
            panel2.add(insetsPanel2, BorderLayout.WEST);
            this.getContentPane().add(panel1, null);
            insetsPanel3.add(label1, null);
            insetsPanel3.add(label2, null);
            insetsPanel3.add(label3, null);
            insetsPanel3.add(label4, null);
            panel2.add(insetsPanel3, BorderLayout.CENTER);
            insetsPanel1.add(button1, null);
            panel1.add(insetsPanel1, BorderLayout.SOUTH);
            panel1.add(panel2, BorderLayout.NORTH);
            setResizable(true);
        } catch (final Exception ex) {
            log.error(ex.getMessage(), ex);
        }
    }

    /**
     * Component initialization.
     * @throws Exception some Exception
     */
    private void jbInit() throws Exception {
    }

    /**
     * Overridden so we can exit when window is closed.
     * @param e Action Event
     */
    protected void processWindowEvent(final WindowEvent e) {
        if (e.getID() == WindowEvent.WINDOW_CLOSING) {
            cancel();
        }
        super.processWindowEvent(e);
    }

    /**
     * Close the dialog.
     */
    void cancel() {
        dispose();
    }

    /**
     * Close the dialog on a button event.
     * @param e Action Event
     */
    public void actionPerformed(final ActionEvent e) {
        if (e.getSource() == button1) {
            cancel();
        }
    }
}