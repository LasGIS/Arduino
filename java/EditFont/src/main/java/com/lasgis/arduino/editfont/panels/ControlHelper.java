/*
 *  @(#)ControlHelper.java  last: 25.08.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.panels;

import com.lasgis.arduino.editfont.data.FontDataPerformed;
import lombok.extern.slf4j.Slf4j;

import javax.swing.JFrame;
import javax.swing.UIManager;
import javax.swing.WindowConstants;
import java.awt.Dimension;
import java.awt.Toolkit;

/**
 * The Class RobotControl.
 *
 * @author Vladimir Laskin
 * @version 1.0
 */
@Slf4j
public final class ControlHelper {
    private ControlHelper() {
    }

    /**
     * Главный запуск программы.
     */
    public static void panel() {

        try {
            JFrame.setDefaultLookAndFeelDecorated(true);
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (final Exception ex) {
            log.error(ex.getMessage(), ex);
        }

        (new ControlHelper()).run();
    }

    private void run() {
        try {
            JFrame.setDefaultLookAndFeelDecorated(true);
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (final Exception ex) {
            log.error(ex.getMessage(), ex);
        }

        final MainFrame frame = new MainFrame();
        FontDataPerformed.addListener(frame.fontDataListener, true);
        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);

        // Validate panels that have preset sizes
        // Pack panels that have useful preferred size info,
        // e.g. from their layout
        frame.validate();

        //Center the window
        final Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        final Dimension frameSize = frame.getSize();
        if (frameSize.height > screenSize.height) {
            frameSize.height = screenSize.height;
        }
        if (frameSize.width > screenSize.width) {
            frameSize.width = screenSize.width;
        }
        frame.setLocation(
            (screenSize.width - frameSize.width) / 2,
            (screenSize.height - frameSize.height) / 2
        );
        frame.setVisible(true);
    }
}
