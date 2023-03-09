/*
 *  @(#)RomFLOAT.java  last: 09.03.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import com.lasgis.util.ByteArrayBuilder;
import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.ToString;

import java.io.UnsupportedEncodingException;

/**
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
@ToString(callSuper = true)
public class RomFLOAT extends RomData {

    private float val;

    private RomFLOAT(final String name, final String refId, final float val) {
        super(name, refId);
        this.val = val;
    }

    /**
     * Статический конструктор.
     * @param val значение
     * @return class instance
     */
    public static RomFLOAT of(final float val) {
        return new RomFLOAT(null, null, val);
    }

    /**
     * Статический конструктор.
     * @param name имя
     * @param val значение
     * @return class instance
     */
    public static RomFLOAT of(final String name, final String refId, final float val) {
        return new RomFLOAT(name, refId, val);
    }

    /**
     * Статический конструктор.
     * @param val значение
     * @return class instance
     */
    public static RomFLOAT of(final double val) {
        return new RomFLOAT(null, null, (float) val);
    }

    /**
     * Статический конструктор.
     * @param name имя
     * @param val значение
     * @return class instance
     */
    public static RomFLOAT of(final String name, final String refId, final double val) {
        return new RomFLOAT(name, refId, (float) val);
    }

    @Override
    public int size() {
        return Float.BYTES;
    }

    @Override
    public Character defChar() {
        return 'f';
    }

    @Override
    public ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        setOffset(buff.position());
        return buff.putFloat(val);
    }
}
