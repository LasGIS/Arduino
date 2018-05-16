/**
 * @(#)RobotControl.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2015 LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.robot;

import javax.swing.JFrame;
import javax.swing.UIManager;
import javax.swing.WindowConstants;
import java.awt.Dimension;
import java.awt.Toolkit;

import com.lasgis.arduino.robot.panels.MainFrame;
import lombok.extern.slf4j.Slf4j;

/**
 * The Class RobotControl.
 * @author Vladimir Laskin
 * @version 1.0
 */
@Slf4j
public final class RobotControl {

    /**
     * Construct the application.
     */
    private RobotControl() {

        final MainFrame frame = new MainFrame();
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

    /**
     * Главный запуск программы.
     * @param args аргументы командной строки
     */
    public static void main(final String[] args) {

        try {
            JFrame.setDefaultLookAndFeelDecorated(true);
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (final Exception ex) {
            log.error(ex.getMessage(), ex);
        }

        new RobotControl();
    }
}