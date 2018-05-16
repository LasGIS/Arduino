package com.lasgis.arduino.eeprom;

import lombok.extern.slf4j.Slf4j;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Enumeration;
import java.util.Properties;

/**
 * Запуск UpLoad.
 *
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Slf4j
public class UpLoad {

    /**
     *
     * на входе следующие параметры:
     *   -port.name COM3
     *   -baud.rate 96000
     *   -patch D:\Arduino\arduino\SmartClock20
     *   -data I2CMemory.data
     *   и / или
     *   -prop SmartClock20.properties
     * который содержит все эти параметры
     * @param args аргументы командной строки
     */
    public UpLoad(final String[] args) throws IOException {
        final Properties prop = loadProp(args);
        Enumeration<Object> keys = prop.keys();
        while (keys.hasMoreElements()) {
            final String key = (String) keys.nextElement();
            final String val = prop.getProperty(key);
            System.out.println(key + "=" + val);
        }
    }

    private Properties loadProp(final String[] args) throws IOException {
        final Properties prop = new Properties();
        String name = "undefine";
        for (final String option : args) {
            if (option.startsWith("-")) {
                name = option.substring(1).trim();
            } else {
                prop.setProperty(name, option);
            }
        }
        final String fileName = prop.getProperty("prop");
        if (fileName != null) {
            final Properties propFin = new Properties();
            final FileInputStream is = new FileInputStream(fileName);
            propFin.load(is);
            is.close();
            propFin.putAll(prop);
            return propFin;
        } else {
            return prop;
        }
    }

    /**
     * Главный запуск программы.
     * @param args аргументы командной строки
     */
    public static void main(final String[] args) {
        try {
            new UpLoad(args);
        } catch (final Exception ex) {
            log.error(ex.getMessage(), ex);
        }
    }
}
