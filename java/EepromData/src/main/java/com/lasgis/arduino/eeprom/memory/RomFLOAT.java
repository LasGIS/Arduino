/*
 * @(#)RomFLOAT.java
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright © 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import com.lasgis.util.ByteArrayBuilder;
import lombok.Data;
import lombok.EqualsAndHashCode;

import java.io.UnsupportedEncodingException;

/**
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
public class RomFLOAT extends RomData {

    private float val;

    /*public RomFLOAT(final String name) {
        super(name);
    }*/

    private RomFLOAT(final String name, final float val) {
        super(name);
        this.val = val;
    }

    /**
     * Статический конструктор.
     * @param val значение
     * @return class instance
     */
    public static RomFLOAT of(final float val) {
        return new RomFLOAT(null, val);
    }

    /**
     * Статический конструктор.
     * @param name имя
     * @param val значение
     * @return class instance
     */
    public static RomFLOAT of(final String name, final float val) {
        return new RomFLOAT(name, val);
    }

    /**
     * Статический конструктор.
     * @param val значение
     * @return class instance
     */
    public static RomFLOAT of(final double val) {
        return new RomFLOAT(null, (float) val);
    }

    /**
     * Статический конструктор.
     * @param name имя
     * @param val значение
     * @return class instance
     */
    public static RomFLOAT of(final String name, final double val) {
        return new RomFLOAT(name, (float) val);
    }

    @Override
    public int size() {
        return Float.BYTES;
    }

    @Override
    public String define() {
        return "f";
    }

    @Override
    public ByteArrayBuilder toEeprom(final ByteArrayBuilder buff) throws UnsupportedEncodingException {
        setOffset(buff.position());
        return buff.putFloat(val);
    }
}
