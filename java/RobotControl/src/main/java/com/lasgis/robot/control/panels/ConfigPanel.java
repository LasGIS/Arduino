/**
 * @(#)ConfigPanel.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2015 LasGIS Company. All Rights Reserved.
 */

package com.lasgis.robot.control.panels;

import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTextArea;
import javax.swing.JTree;
import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.Point;
import java.awt.event.InputEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Панель конфигурации.
 * @author VLaskin
 * @version 1.0 Date: 13.01.2005 16:38:05
 */
public class ConfigPanel extends JPanel implements MouseWheelListener, MouseListener {

    private static final Logger LOG = LoggerFactory.getLogger(ConfigPanel.class);

    /** дерево конфигурации. */
    private JPanel tree;
    /** панель для прокрутки дерева. */
    private JScrollPane treeScroll;
    /** панель для информации об ячейках. */
    private final JTextArea arealInfo = new JTextArea();


    /**
     * Invoked when the mouse wheel is rotated.
     * @param event MouseWheelEvent
     */
    public void mouseWheelMoved(final MouseWheelEvent event) {
        final int modif = event.getModifiersEx();
        final int rotation = event.getWheelRotation();
        /*if ((modif & InputEvent.CTRL_DOWN_MASK) != 0) {
            //
        } else*/
        if ((modif & InputEvent.SHIFT_DOWN_MASK) != 0) {
            final JScrollBar sb = treeScroll.getHorizontalScrollBar();
            final int val = sb.getValue();
            sb.setValue(val + rotation * 15);
        } else {
            final JScrollBar sb = treeScroll.getVerticalScrollBar();
            final int val = sb.getValue();
            sb.setValue(val + rotation * 15);
        }
    }

    /**
     * Invoked when the mouse button has been clicked (pressed and released) on a component.
     * @param event MouseEvent
     */
    public void mouseClicked(final MouseEvent event) {
        final Point pnt = event.getPoint();
        final int button = event.getButton();
        if (button == MouseEvent.BUTTON1) {
            final JTree eventTree = (JTree) event.getSource();
            final int row = eventTree.getLeadSelectionRow();
            if (row >= 0) {
                final Object object = eventTree.getLastSelectedPathComponent();
/*
                if (object instanceof DefaultMutableTreeNode) {
                    final Object userObject = ((DefaultMutableTreeNode) object).getUserObject();
                    if (userObject instanceof LiveObjectElement) {
                        final LiveObjectElement element = (LiveObjectElement) userObject;
                        final TreePath path = eventTree.getSelectionPath();
                        final Rectangle rect = eventTree.getPathBounds(path);
                        if (rect != null) {
                            rect.width = 16;
                            if (rect.contains(pnt)) {
                                element.setShow(!element.isShow());
                                eventTree.repaint();
                            }
                        }
                    }
                }
*/
            }
        }
    }

    /**
     * Invoked when a mouse button has been pressed on a component.
     * @param event MouseEvent
     */
    public void mousePressed(final MouseEvent event) {
        final Point pnt = event.getPoint();
    }

    /**
     * Invoked when a mouse button has been released on a component.
     * @param event MouseEvent
     */
    public void mouseReleased(final MouseEvent event) {
        final Point pnt = event.getPoint();
    }

    /**
     * Method for implements MouseListener.
     * @param event MouseEvent
     */
    public void mouseEntered(final MouseEvent event) {
        final Point pnt = event.getPoint();
    }

    /**
     * Invoked when the mouse exits a component.
     * @param event MouseEvent
     */
    public void mouseExited(final MouseEvent event) {
        final Point pnt = event.getPoint();
    }

    /**
     * Конструктор.
     */
    public ConfigPanel() {
        super();
        tree = new JPanel();
        tree.setBackground(MapPanel.PANEL_GRAY_COLOR);
        treeScroll = new JScrollPane(tree);
        treeScroll.setViewportView(tree);

        final Font font = new Font("Arial", Font.PLAIN, 12);

        /** панель для получении информации от робота. */
        arealInfo.setFont(font);
        arealInfo.setTabSize(16);
        arealInfo.setColumns(2);
        final JScrollPane plantInfoScroll = new JScrollPane(arealInfo);
        plantInfoScroll.setViewportView(arealInfo);

        /* разделительная панелька */
        final JSplitPane splitPane = new JSplitPane(JSplitPane.VERTICAL_SPLIT);
        splitPane.setContinuousLayout(true);
        splitPane.add(treeScroll, JSplitPane.TOP);
        splitPane.add(plantInfoScroll, JSplitPane.BOTTOM);
        splitPane.setDividerLocation(100);
        splitPane.setLastDividerLocation(100);
        splitPane.setResizeWeight(0.0);

        setLayout(new BorderLayout());
        /* панель режима. */
        add(splitPane, BorderLayout.CENTER);

        tree.addMouseWheelListener(this);
        tree.addMouseListener(this);

    }

    public JTextArea getArealInfo() {
        return arealInfo;
    }
}
