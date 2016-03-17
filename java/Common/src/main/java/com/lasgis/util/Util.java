/**
 * @(#)Util.java 1.0
 *
 * Title: LG Evolution powered by Java
 * Description: Program for imitation of evolutions process.
 * Copyright (c) 2012-2015 LasGIS Company. All Rights Reserved.
 */

package com.lasgis.util;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import java.awt.Dimension;
import java.awt.Image;
import java.awt.event.ActionListener;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Created by IntelliJ IDEA.
 * @author VLaskin
 * @version 1.0
 * Date: Sep 3, 2004
 * Time: 6:20:54 PM
 */
public final class Util {

    private static final Logger LOG = LoggerFactory.getLogger(Util.class);
    private static final int BUFF_SIZE = 1024;

    /**
     * Скрываем конструктор.
     */
    private Util() {
    }

    /**
     * Загрузит иконку из каталога image.
     * @param name имя иконки
     * @return ImageIcon
     * @throws IOException Signals that an I/O exception of some sort has occurred.
     */
    public static ImageIcon loadImageIcon(final String name) throws IOException {
        return new ImageIcon(loadImage(name));
    }

    /**
     * Загрузит иконку из каталога image.
     * @param name имя иконки
     * @return ImageIcon
     * @throws IOException Signals that an I/O exception of some sort has occurred.
     */
    public static Image loadImage(final String name) throws IOException {
        final ClassLoader ldr = Util.class.getClassLoader();
        final InputStream in = ldr.getResourceAsStream("image/" + name);
        if (in != null) {
            try {
                return ImageIO.read(in);
            } finally {
                in.close();
            }
        } else {
            throw new IOException("Файл \"" + "image/" + name + "\" не найден!");
        }
    }

    /**
     * Загрузит ресурс из classpath.
     * @param name имя иконки
     * @return ImageIcon
     */
    public static StringBuilder loadString(final String name) {
        final ClassLoader ldr = Util.class.getClassLoader();
        final InputStream in = ldr.getResourceAsStream(name);
        final StringBuilder sb = new StringBuilder();
        if (in != null) {
            try (InputStreamReader reader = new InputStreamReader(in, "UTF-8")) {
                final char[] buf = new char[BUFF_SIZE];
                int count = reader.read(buf, 0, BUFF_SIZE);
                while (count >= 0) {
                    sb.append(buf, 0, count);
                    count = reader.read(buf, 0, BUFF_SIZE);
                }
                return sb;
            } catch (final IOException ex) {
                LOG.error(ex.getMessage(), ex);
            }
        } else {
            LOG.error("Файл \"" + name + "\" не найден!");
        }
        return null;
    }

    /**
     * Загрузит ресурс из classpath.
     * @param fileName имя иконки
     * @return ImageIcon
     */
    public static StringBuilder loadStringFromFile(final String fileName)  {
        try (
            InputStream in = new FileInputStream(fileName);
            InputStreamReader reader = new InputStreamReader(in, "UTF-8");
        ) {
            final StringBuilder sb = new StringBuilder();
            final char[] buf = new char[BUFF_SIZE];
            int count = reader.read(buf, 0, BUFF_SIZE);
            while (count >= 0) {
                sb.append(buf, 0, count);
                count = reader.read(buf, 0, BUFF_SIZE);
            }
            return sb;
        } catch (final FileNotFoundException ex) {
            LOG.error("Файл \"" + fileName + "\" не найден!");
        } catch (final IOException ex) {
            LOG.error(ex.getMessage(), ex);
        }
        return null;
    }

    /**
     * создать кнопку.
     * @param name надпись на кнопке
     * @param iconName иконка на кнопке
     * @param width ширина кнопки
     * @param height высота кнопки
     * @param toolTip всплывающая подсказка
     * @param actListener действие на кнопку
     * @return created JButton
     */
    public static JButton createImageButton(
        final String name,
        final String iconName,
        final int width, final int height,
        final String toolTip,
        final ActionListener actListener
    ) {
        final JButton button = new JButton();
        if (name != null) {
            button.setText(name);
        }
        if (iconName != null) {
            try {
                button.setIcon(Util.loadImageIcon(iconName));
            } catch (final IOException e) {
                LoggerFactory.getLogger(Util.class).error(e.getMessage());
            }
        }
//        button.setMaximumSize(new Dimension(width, height));
        button.setMinimumSize(new Dimension(width, height));
        button.setPreferredSize(new Dimension(width, height));
        button.setToolTipText(toolTip);
        button.addActionListener(actListener);
        return button;
    }

    /**
     * создать кнопку.
     * @param set настройки для создания кнопки
     * @return created JButton
     */
    public static JButton createImageButton(final SettingToolBarItem set) {
        final JButton button = new JButton();
        if (set.getText() != null) {
            button.setText(set.getText());
        }
        if (set.getImage() != null) {
            try {
                button.setIcon(Util.loadImageIcon(set.getImage()));
            } catch (final IOException e) {
                LoggerFactory.getLogger(Util.class).error(e.getMessage());
            }
        }
        final Dimension dim = new Dimension(set.getWidth(), set.getHeight());
//        button.setMaximumSize(new Dimension(width, height));
        button.setMinimumSize(dim);
        button.setPreferredSize(dim);
        button.setToolTipText(set.getToolTip());
        button.addActionListener(set.getAction());
        return button;
    }

    /**
     * Создаём и возвращаем объект типа JMenu или JMenuItem.
     * @param set объект типа SettingMenuItem, содержащий информацию для создания меню
     * @return объект типа JMenu или JMenuItem
     */
    public static JMenuItem createImageMenuItem(
        final SettingMenuItem set
    ) {
        if (set.getItems() == null) {
            // создаём одну строку меню
            final JMenuItem menu = new JMenuItem();
            menu.setText(set.getText());
            if (set.getImage() != null) {
                try {
                    menu.setIcon(Util.loadImageIcon(set.getImage()));
                } catch (final IOException e) {
                    LoggerFactory.getLogger(Util.class).error(e.getMessage());
                }
            }
            menu.setToolTipText(set.getToolTip());
            menu.addActionListener(set.getAction());
            return menu;
        } else {
            // создаём выпадающее подменю
            final JMenu menu = new JMenu();
            menu.setText(set.getText());
            if (set.getImage() != null) {
                try {
                    menu.setIcon(Util.loadImageIcon(set.getImage()));
                } catch (final IOException e) {
                    LoggerFactory.getLogger(Util.class).error(e.getMessage());
                }
            }
            menu.setToolTipText(set.getToolTip());
            final SettingMenuItem[] setts = set.getItems();
            for (SettingMenuItem sett : setts) {
                menu.add(createImageMenuItem(sett));
            }
            return menu;
        }
    }

}