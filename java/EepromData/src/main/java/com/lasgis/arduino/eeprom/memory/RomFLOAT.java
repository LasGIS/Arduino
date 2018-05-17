/*
 *  @(#)RomFLOAT.java  last: 17.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.memory;

import lombok.Data;
import lombok.EqualsAndHashCode;

import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;

/**
 * @author Vladimir Laskin
 * @since 17.05.2018
 */
@Data
@EqualsAndHashCode(callSuper = true)
public class RomFLOAT extends RomData {

    private float val;

    public RomFLOAT(final String name) {
        super(name);
    }

    private RomFLOAT(final String name, final float val) {
        super(name);
        this.val = val;
    }

    static RomFLOAT of(final float val) {
        return new RomFLOAT(null, val);
    }

    static RomFLOAT of(final String name, final float val) {
        return new RomFLOAT(name, val);
    }

    @Override
    byte[] toByte() throws UnsupportedEncodingException {
        byte[] bytes = new byte[4];
        ByteBuffer.wrap(bytes).putFloat(val);
        return bytes;
    }
}
