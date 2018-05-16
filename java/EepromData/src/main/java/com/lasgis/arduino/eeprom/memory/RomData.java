package com.lasgis.arduino.eeprom.memory;

import lombok.Data;

import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;

/**
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Data
public abstract class RomData {
    protected static final Charset CHARSET = Charset.forName("windows-1251");
    private String name;
    abstract byte[] toByte() throws UnsupportedEncodingException;
}
