package com.lasgis.arduino.eeprom.upload;

import com.lasgis.util.ByteArrayBuilder;
import lombok.Data;

/**
 *
 *
 * @author Vladimir Laskin
 * @since 04.06.2018
 */
@Data
public class SerialBlock {
    /** Размер блока */
    byte size = 0;
    /** номер микосхемы (0x57 для CMOS) */
    byte device = 0x57;
    /** адрес блока в EEPROM памяти */
    short address;
    /** Контрольная сумма блока */
    short cs;
    /** тело блока */
    byte[] body;
    /** true, если этот блок ещё не передали */
    private boolean processed = true;

    public byte[] getBytes() {
        final ByteArrayBuilder bab = new ByteArrayBuilder(10 + size);
        bab.put(':').put('B').put('W').put(size).put(device).putShort(address).putShort(cs).put(body);
        return bab.toByte();
    }
}
