/*
 *  @(#)ByteArrayBuilderTest.java  last: 21.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.util;

import org.testng.annotations.Test;

import javax.xml.bind.DatatypeConverter;

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
        bab.put((byte) 0x00).put((byte) 0x21).put((byte) 0x00).put((byte) 0x23)
            .put(0, (byte) 0x20).put(2, (byte) 0x22);
        assertEquals(DatatypeConverter.printHexBinary(bab.toByte()), "20212223");
        assertEquals(bab.position(), 4);
        bab.position(0);
        assertEquals(bab.get(), (byte) 0x20);
        assertEquals(bab.get(), (byte) 0x21);
        assertEquals(bab.get(), (byte) 0x22);
        assertEquals(bab.get(), (byte) 0x23);
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
/*
    @Test
    public void testGetChar() throws Exception {
    }
    @Test
    public void testPutChar() throws Exception {
    }
    @Test
    public void testAsCharBuffer() throws Exception {
    }

    @Test
    public void testGetShort() throws Exception {
    }
    @Test
    public void testPutShort() throws Exception {
    }
    @Test
    public void testAsShortBuffer() throws Exception {
    }

    @Test
    public void testGetInt() throws Exception {
    }
    @Test
    public void testPutInt() throws Exception {
    }
    @Test
    public void testAsIntBuffer() throws Exception {
    }

    @Test
    public void testGetLong() throws Exception {
    }
    @Test
    public void testPutLong() throws Exception {
    }
    @Test
    public void testAsLongBuffer() throws Exception {
    }

    @Test
    public void testGetFloat() throws Exception {
    }
    @Test
    public void testPutFloat() throws Exception {
    }
    @Test
    public void testAsFloatBuffer() throws Exception {
    }

    @Test
    public void testGetDouble() throws Exception {
    }
    @Test
    public void testPutDouble() throws Exception {
    }
    @Test
    public void testAsDoubleBuffer() throws Exception {
    }
*/
}