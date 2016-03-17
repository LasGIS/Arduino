/**
 * @(#)ConfigPanel.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2015 LasGIS Company. All Rights Reserved.
 */

package com.lasgis.robot.control.panels;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionListener;

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

    /** дерево конфигурации. */
    private final JPanel controlPanel = new JPanel(new BorderLayout());
    /** панель для информации об ячейках. */
    private final JTextArea arealInfo = new JTextArea();

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

    private void fillControlPanel() {
        controlPanel.setBackground(MapPanel.PANEL_GRAY_COLOR);

        final JPanel keyPanel = new JPanel(new GridLayout(3, 3, 5, 5));
        keyPanel.setSize(100, 100);
        final JButton forward = createNavigationButton("arrow_up.gif", "вперед", null);
        final JButton backward = createNavigationButton("arrow_down.gif", "назад", null);
        final JButton toLeft = createNavigationButton("arrow_left.gif", "влево", null);
        final JButton toRight = createNavigationButton("arrow_right.gif", "вправо", null);
/*
        arrow_down_left.gif
        arrow_down_right.gif
        arrow_up_left.gif
        arrow_up_right.gif
*/
        keyPanel.add(new JLabel("TL", JLabel.CENTER));
        keyPanel.add(forward);
        keyPanel.add(new JLabel("TR", JLabel.CENTER ));
        keyPanel.add(toLeft);
        keyPanel.add(new JLabel("SC", JLabel.CENTER));
        keyPanel.add(toRight);
        keyPanel.add(new JLabel("BL", JLabel.CENTER));
        keyPanel.add(backward);
        keyPanel.add(new JLabel("BR", JLabel.CENTER));
        controlPanel.add(keyPanel, BorderLayout.EAST);
/*
        controlPanel.add(new JPanel(true), BorderLayout.EAST);
        controlPanel.add(new JPanel(false), BorderLayout.WEST);
*/

        final JTextField command = new JTextField();
        command.addActionListener(e -> {
            LOG.debug(e.getActionCommand());
            LOG.debug("Class = " + e.getSource().getClass().getName());
            ((JTextField) e.getSource()).setText("");
        });
        controlPanel.add(command, BorderLayout.SOUTH);

    }

    private JButton createNavigationButton(
        final String iconName, final String toolTip, final ActionListener actListener
    ) {
        final JButton button = createImageButton(null, iconName, 24, 24, toolTip, actListener);
        button.setBorderPainted(false);
        return button;
    }

    public JTextArea getArealInfo() {
        return arealInfo;
    }
}
