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
public class RomINT32 extends RomData {

    private int val;

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        final byte[] out = new byte[4];
        int valOut = val;
        for (int i = 0; i < 4; i++) {
            out[i] = (byte) (valOut & 0xff);
            valOut >>= 8;
        }
        return out;
    }
}
