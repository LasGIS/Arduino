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
public class RomINT8 extends RomData {

    private byte val;

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        final byte[] out = new byte[1];
        out[0] = val;
        return out;
    }
}
