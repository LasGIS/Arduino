/*
 *  @(#)SerialBlock.java  last: 06.02.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.upload;

import com.lasgis.util.ByteArrayBuilder;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

/**
 * @author Vladimir Laskin
 * @since 04.06.2018
 */
@Data
@AllArgsConstructor
@NoArgsConstructor
public class SerialBlock {
    /** номер микросхемы (0x57 для CMOS) */
    byte device = 0x57;
    /** адрес блока в EEPROM памяти */
    short address;
    /** Размер блока */
    short size = 0;
    /** Контрольная сумма блока */
    short cs;
    /** тело блока */
    byte[] body;
    /** true, если этот блок ещё не передали */
    private boolean processed = true;
    /** true, если пришло подтверждение о загрузке */
    private boolean uploaded = false;

    public static SerialBlock of(final byte device, final short address, final short size, final short cs) {
        return new SerialBlock(device, address, size, cs, null, true, false);
    }

    public byte[] getWriteBytes() {
        final ByteArrayBuilder bab = new ByteArrayBuilder(11 + size);
        bab.put(':').put('B').put('W').put(device).putShort(address).putShort(size).putShort(cs).put(body);
        return bab.toByte();
    }

    public byte[] getHead4ReadBytes() {
        final ByteArrayBuilder bab = new ByteArrayBuilder(11 + size);
        bab.put(':').put('B').put('R').put(device).putShort(address).putShort(size).putShort(cs);
        return bab.toByte();
    }
}
