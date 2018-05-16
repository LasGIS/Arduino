package com.lasgis.arduino.eeprom;

/**
 * Для вывода без StackTrace
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
public class UpLoadInfoException extends RuntimeException {
    public UpLoadInfoException(final String message) {
        super(message);
    }
}
