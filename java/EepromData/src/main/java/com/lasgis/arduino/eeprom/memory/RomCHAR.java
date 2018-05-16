package com.lasgis.arduino.eeprom.memory;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.io.UnsupportedEncodingException;

/**
 * @author Vladimir Laskin
 * @since 16.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
@AllArgsConstructor(staticName = "of")
public class RomCHAR extends RomData {

    private char val;

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        return Character.toString(val).getBytes(CHARSET);
    }
}
