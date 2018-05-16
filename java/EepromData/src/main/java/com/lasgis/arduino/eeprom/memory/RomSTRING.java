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
public class RomSTRING extends RomData {

    private String val;

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        final int count = val.length();
        final byte[] str = val.getBytes(CHARSET);
        final byte[] out = new byte[count + 2];
        out[1] = (byte) (count & 0xff);
        out[0] = (byte) ((count >> 8) & 0xff);
        for (int i = 0, j = 2; i < count; i++, j++) {
            out[j] = str[i];
        }
        return out;
    }
}
