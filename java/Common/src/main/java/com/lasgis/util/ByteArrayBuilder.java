/*
 *  @(#)ByteArrayBuilder.java  last: 21.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.util;

import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.DoubleBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.nio.ShortBuffer;

/**
 * @author Vladimir Laskin
 * @since 18.05.2018
 */
public class ByteArrayBuilder {

    private ByteBuffer buffer;

    public ByteArrayBuilder(int capacity) {
        super();
        buffer = ByteBuffer.allocate(capacity);
    }

    public ByteArrayBuilder() {
        this(1024);
    }

    public ByteArrayBuilder slice() {
        buffer = buffer.slice();
        return this;
    }

    public ByteArrayBuilder duplicate() {
        buffer = buffer.duplicate();
        return this;
    }

    public ByteArrayBuilder asReadOnlyBuffer() {
        buffer = buffer.asReadOnlyBuffer();
        return this;
    }

    public byte get() {
        return buffer.get();
    }

    public byte get(int i) {
        return buffer.get(i);
    }

    public ByteArrayBuilder get(byte[] dst, int offset, int length) {
        buffer.get(dst, offset, length);
        return this;
    }

    public ByteArrayBuilder put(byte x) {
        buffer.put(x);
        return this;
    }

    public ByteArrayBuilder put(int i, byte x) {
        buffer.put(i,x);
        return this;
    }

    public ByteArrayBuilder put(byte[] src, int offset, int length) {
        buffer.put(src,offset,length);
        return this;
    }

    public ByteArrayBuilder put(ByteBuffer src) {
        buffer.put(src);
        return this;
    }

    public ByteArrayBuilder compact() {
        buffer.compact();
        return this;
    }

    public char getChar() {
        return buffer.getChar();
    }

    public char getChar(int i) {
        return buffer.getChar(i);
    }

    public ByteArrayBuilder putChar(char x) {
        buffer.putChar(x);
        return this;
    }

    public ByteArrayBuilder putChar(int i, char x) {
        buffer.putChar(i, x);
        return this;
    }

    public CharBuffer asCharBuffer() {
        return buffer.asCharBuffer();
    }

    public short getShort() {
        return buffer.getShort();
    }

    public short getShort(int i) {
        return buffer.getShort(i);
    }

    public ByteArrayBuilder putShort(short x) {
        buffer.putShort(x);
        return this;
    }

    public ByteArrayBuilder putShort(int i, short x) {
        buffer.putShort(i, x);
        return this;
    }

    public ShortBuffer asShortBuffer() {
        return buffer.asShortBuffer();
    }

    public int getInt() {
        return buffer.getInt();
    }

    public int getInt(int i) {
        return buffer.getInt(i);
    }


    public ByteArrayBuilder putInt(int x) {
        buffer.putInt(x);
        return this;
    }

    public ByteArrayBuilder putInt(int i, int x) {
        buffer.putInt(i, x);
        return this;
    }

    public IntBuffer asIntBuffer() {
        return buffer.asIntBuffer();
    }

    public long getLong() {
        return buffer.getLong();
    }

    public long getLong(int i) {
        return buffer.getLong(i);
    }

    public ByteArrayBuilder putLong(long x) {
        buffer.putLong(x);
        return this;
    }

    public ByteArrayBuilder putLong(int i, long x) {
        buffer.putLong(i, x);
        return this;
    }

    public LongBuffer asLongBuffer() {
        return buffer.asLongBuffer();
    }

    public float getFloat() {
        return buffer.getFloat();
    }

    public float getFloat(int i) {
        return buffer.getFloat(i);
    }

    public ByteArrayBuilder putFloat(float x) {
        buffer.putFloat(x);
        return this;
    }

    public ByteArrayBuilder putFloat(int i, float x) {
        buffer.putFloat(i, x);
        return this;
    }

    public FloatBuffer asFloatBuffer() {
        return buffer.asFloatBuffer();
    }

    public double getDouble() {
        return buffer.getDouble();
    }

    public double getDouble(int i) {
        return buffer.getDouble(i);
    }

    public ByteArrayBuilder putDouble(double x) {
        buffer.putDouble(x);
        return this;
    }

    public ByteArrayBuilder putDouble(int i, double x) {
        buffer.putDouble(i, x);
        return this;
    }

    public DoubleBuffer asDoubleBuffer() {
        return buffer.asDoubleBuffer();
    }
}
