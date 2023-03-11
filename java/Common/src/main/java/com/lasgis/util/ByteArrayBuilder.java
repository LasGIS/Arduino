/*
 *  @(#)ByteArrayBuilder.java  last: 12.03.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.util;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
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
        buffer.order(ByteOrder.LITTLE_ENDIAN);
    }

    public ByteArrayBuilder() {
        this(10);
    }

    public final int position() {
        return buffer.position();
    }

    /**
     * Sets this buffer's position.
     *
     * @param newPosition The new position value; must be non-negative and no larger than the current limit
     * @return This buffer
     */
    public final ByteArrayBuilder position(int newPosition) {
        buffer.position(newPosition);
        return this;
    }

    /**
     * Получаем байт по текущему индексу
     *
     * @return байт по текущему индексу
     */
    public byte get() {
        return buffer.get();
    }

    /**
     * Получаем байт по индексу 'index'
     *
     * @param index индекс
     * @return байт по индексу 'index'
     */
    public byte get(int index) {
        return buffer.get(index);
    }

    /**
     * Передаем байты из этого буфера в заданный массив назначения
     *
     * @param dst    dst массив назначения (должен быть создан заранее)
     * @param offset смещение в своем массиве
     * @param length длинна передаваемой посылке
     * @return сам себя (ByteArrayBuilder)
     */
    public ByteArrayBuilder get(byte[] dst, int offset, int length) {
        buffer.get(dst, offset, length);
        return this;
    }

    /**
     * Устанавливаем байт 'x' по текущему индексу
     *
     * @param x байт
     * @return сам себя (ByteArrayBuilder)
     */
    public ByteArrayBuilder put(byte x) {
        checkLimit(1);
        buffer.put(x);
        return this;
    }

    /**
     * Устанавливаем байт 'x' по текущему индексу
     *
     * @param x целое как байт
     * @return сам себя (ByteArrayBuilder)
     */
    public ByteArrayBuilder put(int x) {
        return put((byte) x);
    }

    /**
     * Устанавливаем байт 'x' по индексу 'index'
     *
     * @param index индекс
     * @param x     байт
     * @return сам себя (ByteArrayBuilder)
     */
    public ByteArrayBuilder put(int index, byte x) {
        checkLimit(index, 1);
        buffer.put(index, x);
        return this;
    }

    /**
     * Устанавливаем байт 'x' по индексу 'index'
     *
     * @param index индекс
     * @param x     целое как байт
     * @return сам себя (ByteArrayBuilder)
     */
    public ByteArrayBuilder put(int index, int x) {
        return put(index, (byte) x);
    }


    public ByteArrayBuilder put(byte[] src, int offset, int length) {
        checkLimit(length);
        buffer.put(src, offset, length);
        return this;
    }

    public ByteArrayBuilder put(byte[] src) {
        return put(src, 0, src.length);
    }

    public ByteArrayBuilder put(ByteBuffer src) {
        checkLimit(src.remaining());
        buffer.put(src);
        return this;
    }

    public char getChar() {
        return buffer.getChar();
    }

    public char getChar(int index) {
        return buffer.getChar(index);
    }

    public ByteArrayBuilder putChar(char x) {
        checkLimit(2);
        buffer.putChar(x);
        return this;
    }

    public ByteArrayBuilder putChar(int index, char x) {
        checkLimit(index, 2);
        buffer.putChar(index, x);
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
        checkLimit(2);
        buffer.putShort(x);
        return this;
    }

    public ByteArrayBuilder putShort(int x) {
        return putShort((short) x);
    }

    public ByteArrayBuilder putShort(int index, short x) {
        checkLimit(index, 2);
        buffer.putShort(index, x);
        return this;
    }

    public ByteArrayBuilder putShort(int index, int x) {
        return putShort(index, (short) x);
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
        checkLimit(4);
        buffer.putInt(x);
        return this;
    }

    public ByteArrayBuilder putInt(int index, int x) {
        checkLimit(index, 4);
        buffer.putInt(index, x);
        return this;
    }

    public IntBuffer asIntBuffer() {
        return buffer.asIntBuffer();
    }

    public long getLong() {
        return buffer.getLong();
    }

    public long getLong(int index) {
        return buffer.getLong(index);
    }

    public ByteArrayBuilder putLong(long x) {
        checkLimit(8);
        buffer.putLong(x);
        return this;
    }

    public ByteArrayBuilder putLong(int index, long x) {
        checkLimit(index, 8);
        buffer.putLong(index, x);
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
        checkLimit(4);
        buffer.putFloat(x);
        return this;
    }

    public ByteArrayBuilder putFloat(int index, float x) {
        checkLimit(index, 4);
        buffer.putFloat(index, x);
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
        checkLimit(8);
        buffer.putDouble(x);
        return this;
    }

    public ByteArrayBuilder putDouble(int index, double x) {
        checkLimit(index, 8);
        buffer.putDouble(index, x);
        return this;
    }

    public DoubleBuffer asDoubleBuffer() {
        return buffer.asDoubleBuffer();
    }

    public byte[] toByte() {
        final int position = buffer.position();
        final byte[] out = new byte[position];
        buffer.position(0);
        buffer.get(out);
        return out;
    }

    private void checkLimit(final int addSize) {
        increaseCapacity(buffer.position(), addSize);
    }

    private void checkLimit(final int index, final int addSize) {
        increaseCapacity(index, addSize);
    }

    private void increaseCapacity(final int index, final int addSize) {
        if (index + addSize >= buffer.capacity()) {
            final int capacity = index + addSize + 10;
            final ByteBuffer newBuffer = ByteBuffer.allocate(capacity);
            newBuffer.order(ByteOrder.LITTLE_ENDIAN);
            newBuffer.put(buffer.array());
            newBuffer.position(buffer.position());
            buffer = newBuffer;
        }
    }
}
