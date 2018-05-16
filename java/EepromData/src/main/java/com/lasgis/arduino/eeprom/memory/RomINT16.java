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
public class RomINT16 extends RomData {

    private short val;

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        final byte[] out = new byte[2];
        short valOut = val;
        for (int i = 0; i < 2; i++) {
            out[i] = (byte) (valOut & 0xff);
            valOut >>= 8;
        }
        return out;
    }
}
