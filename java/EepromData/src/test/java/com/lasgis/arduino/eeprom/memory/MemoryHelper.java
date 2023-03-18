/*
 *  @(#)MemoryHelper.java  last: 19.03.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import com.lasgis.util.ByteArrayBuilder;

import java.io.UnsupportedEncodingException;
import java.util.HashMap;
import java.util.Map;

/**
 * The Class MemoryHelper definition.
 *
 * @author Vladimir Laskin
 * @since 19.03.2023 : 1:23
 */
public final class MemoryHelper {
    private MemoryHelper() {
    }

    /**
     * <pre>
     * Получаем образ объекта в массив байт.
     * В дальнейшем upload образ в arduino.
     * </pre>
     *
     * @return массив байт
     * @throws UnsupportedEncodingException если ошибка
     */
    public static byte[] toEeprom(final RomData rom) throws UnsupportedEncodingException {
        final ByteArrayBuilder bab = new ByteArrayBuilder();
        final Map<String, AddressToRoms> reference2Address = new HashMap<>();
        return rom.toEeprom(bab, 0x0000, reference2Address).toByte();
    }
}
