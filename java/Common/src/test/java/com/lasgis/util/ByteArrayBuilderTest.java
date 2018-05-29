/*
 *  @(#)ByteArrayBuilderTest.java  last: 29.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.util;

import org.testng.annotations.DataProvider;
import org.testng.annotations.Test;

import javax.xml.bind.DatatypeConverter;
import java.nio.CharBuffer;
import java.nio.DoubleBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.nio.ShortBuffer;

import static org.testng.Assert.assertEquals;

/**
 * @author Vladimir Laskin
 * @since 21.05.2018
 */
public class ByteArrayBuilderTest {

    private final static byte[] heap = {
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    };

    @Test
    public void testPutGet() throws Exception {
        final ByteArrayBuilder bab = new ByteArrayBuilder();
        bab.put(0x00).put(0x21).put(0x00).put(0x23)
            .put(0, 0x20).put(2, 0x22);
        assertEquals(DatatypeConverter.printHexBinary(bab.toByte()), "20212223");
        assertEquals(bab.position(), 4);
        bab.position(0);
        assertEquals(bab.get(), 0x20);
        assertEquals(bab.get(), 0x21);
        assertEquals(bab.get(), 0x22);
        assertEquals(bab.get(), 0x23);
        assertEquals(bab.position(), 4);
    }

    @Test
    public void testCapacity() throws Exception {
        final ByteArrayBuilder bab = new ByteArrayBuilder();
        for (int i = 0; i < heap.length; i++) {
            assertEquals(bab.position(), i);
            bab.put(heap[i]);
        }
        assertEquals(DatatypeConverter.printHexBinary(bab.toByte()), "202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F");
        assertEquals(bab.position(), heap.length);
    }

    @DataProvider
    public Object[][] dataChar() {
        return new Object[][] {
            { new char[]{' ', '3', '\'', '\"'}, "0020003300270022" },
            { new char[]{'A', 'B', 'C', 'Z'}, "004100420043005A" },
            { new char[]{'а', 'б', 'в', 'я'}, "043004310432044F" }
        };
    }
    @Test(dataProvider = "dataChar")
    public void testChar(
        final char[] chars, final String expected
    ) throws Exception {
        final ByteArrayBuilder bab = new ByteArrayBuilder();
        for (final char chr : chars) {
            bab.putChar(chr);
        }
        assertEquals(DatatypeConverter.printHexBinary(bab.toByte()), expected);
        bab.position(0);
        for (final char chr : chars) {
            assertEquals(bab.getChar(), chr);
        }
        for (int i = 0; i < chars.length; i++) {
            assertEquals(bab.getChar(i * Character.BYTES), chars[i]);
        }
        bab.position(0);
        final CharBuffer db = bab.asCharBuffer();
        for (final char aChar : chars) {
            assertEquals(db.get(), aChar);
        }
    }

    @DataProvider
    public Object[][] dataShort() {
        return new Object[][] {
            { new short[]{0, 1, Short.MAX_VALUE, Short.MIN_VALUE, -1, -2}, "000000017FFF8000FFFFFFFE" }
        };
    }
    @Test(dataProvider = "dataShort")
    public void testShort(
        final short[] shorts, final String expected
    ) throws Exception {
        final ByteArrayBuilder bab = new ByteArrayBuilder();
        for (final short val : shorts) {
            bab.putShort(val);
        }
        assertEquals(DatatypeConverter.printHexBinary(bab.toByte()), expected);
        bab.position(0);
        for (final short val : shorts) {
            assertEquals(bab.getShort(), val);
        }
        for (int i = 0; i < shorts.length; i++) {
            assertEquals(bab.getShort(i * Short.BYTES), shorts[i]);
        }
        bab.position(0);
        final ShortBuffer db = bab.asShortBuffer();
        for (final short aShort : shorts) {
            assertEquals(db.get(), aShort);
        }
    }

    @DataProvider
    public Object[][] dataInt() {
        return new Object[][] { {
            new int[]{0, 1, Integer.MAX_VALUE, Integer.MIN_VALUE, -1, -2},
            "00000000000000017FFFFFFF80000000FFFFFFFFFFFFFFFE"
        } };
    }
    @Test(dataProvider = "dataInt")
    public void testInt(
        final int[] ints, final String expected
    ) throws Exception {
        final ByteArrayBuilder bab = new ByteArrayBuilder();
        for (final int val : ints) {
            bab.putInt(val);
        }
        assertEquals(DatatypeConverter.printHexBinary(bab.toByte()), expected);
        bab.position(0);
        for (final int val : ints) {
            assertEquals(bab.getInt(), val);
        }
        for (int i = 0; i < ints.length; i++) {
            assertEquals(bab.getInt(i * Integer.BYTES), ints[i]);
        }
        bab.position(0);
        final IntBuffer db = bab.asIntBuffer();
        for (final int aInteger : ints) {
            assertEquals(db.get(), aInteger);
        }
    }

    @DataProvider
    public Object[][] dataLong() {
        return new Object[][] { {
            new long[]{0, 1, Long.MAX_VALUE, Long.MIN_VALUE, -1, -2},
            "000000000000000000000000000000017FFFFFFFFFFFFFFF"
          + "8000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE"
        } };
    }
    @Test(dataProvider = "dataLong")
    public void testLong(
        final long[] longs, final String expected
    ) throws Exception {
        final ByteArrayBuilder bab = new ByteArrayBuilder();
        for (final long val : longs) {
            bab.putLong(val);
        }
        assertEquals(DatatypeConverter.printHexBinary(bab.toByte()), expected);
        bab.position(0);
        for (final long val : longs) {
            assertEquals(bab.getLong(), val);
        }
        for (int i = 0; i < longs.length; i++) {
            assertEquals(bab.getLong(i * Long.BYTES), longs[i]);
        }
        bab.position(0);
        final LongBuffer db = bab.asLongBuffer();
        for (final long aLong : longs) {
            assertEquals(db.get(), aLong);
        }
    }

    @DataProvider
    public Object[][] dataFloat() {
        return new Object[][] {{
            new float[]{0, 1, Float.MAX_VALUE, Float.MIN_VALUE, -1, -2},
            "00000000" + "3F800000" + "7F7FFFFF" + "00000001" + "BF800000" + "C0000000"
        }, {
            new float[]{10, 0.1f, 0.01f, -10, -0.1f, -0.01f},
            "41200000" + "3DCCCCCD" + "3C23D70A" + "C1200000" + "BDCCCCCD" + "BC23D70A"
        } };
    }
    @Test(dataProvider = "dataFloat")
    public void testFloat(
        final float[] floats, final String expected
    ) throws Exception {
        final ByteArrayBuilder bab = new ByteArrayBuilder();
        for (final float val : floats) {
            bab.putFloat(val);
        }
        assertEquals(DatatypeConverter.printHexBinary(bab.toByte()), expected);
        bab.position(0);
        for (final float val : floats) {
            assertEquals(bab.getFloat(), val);
        }
        for (int i = 0; i < floats.length; i++) {
            assertEquals(bab.getFloat(i * Float.BYTES), floats[i]);
        }
        bab.position(0);
        final FloatBuffer db = bab.asFloatBuffer();
        for (final float aFloat : floats) {
            assertEquals(db.get(), aFloat);
        }
    }

    @DataProvider
    public Object[][] dataDouble() {
        return new Object[][] { {
            new double[]{0, 1, Double.MAX_VALUE, Double.MIN_VALUE, -1, -2},
            "0000000000000000" + "3FF0000000000000" + "7FEFFFFFFFFFFFFF"
          + "0000000000000001" + "BFF0000000000000" + "C000000000000000"
        }, {
            new double[]{10, 0.1, 0.01, -10, -0.1, -0.01},
            "4024000000000000" + "3FB999999999999A" + "3F847AE147AE147B"
          + "C024000000000000" + "BFB999999999999A" + "BF847AE147AE147B"
        } };
    }
    @Test(dataProvider = "dataDouble")
    public void testDouble(
        final double[] doubles, final String expected
    ) throws Exception {
        final ByteArrayBuilder bab = new ByteArrayBuilder();
        for (final double val : doubles) {
            bab.putDouble(val);
        }
        assertEquals(DatatypeConverter.printHexBinary(bab.toByte()), expected);
        bab.position(0);
        for (final double val : doubles) {
            assertEquals(bab.getDouble(), val);
        }
        for (int i = 0; i < doubles.length; i++) {
            assertEquals(bab.getDouble(i * Double.BYTES), doubles[i]);
        }
        bab.position(0);
        final DoubleBuffer db = bab.asDoubleBuffer();
        for (final double aDouble : doubles) {
            assertEquals(db.get(), aDouble);
        }
    }
}