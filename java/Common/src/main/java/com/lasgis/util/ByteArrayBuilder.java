/*
 *  @(#)ByteArrayBuilder.java  last: 18.05.2018
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2018, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.util;

import java.nio.*;

/**
 * @author Vladimir Laskin
 * @since 18.05.2018
 */
public class ByteArrayBuilder extends ByteBuffer {

    private ByteBuffer buffer;

    public ByteArrayBuilder(int capacity) {
        super();
        buffer = ByteBuffer.allocate(capacity);
    }

    public ByteArrayBuilder() {
        this(1024);
    }

    public ByteBuffer slice() {
        buffer = buffer.slice();
        return this;
    }

    public ByteBuffer duplicate() {
        return new java.nio.HeapByteBuffer(hb,
            this.markValue(),
            this.position(),
            this.limit(),
            this.capacity(),
            offset);
    }

    public ByteBuffer asReadOnlyBuffer() {

        return new HeapByteBufferR(hb,
            this.markValue(),
            this.position(),
            this.limit(),
            this.capacity(),
            offset);


    }


    protected int ix(int i) {
        return i + offset;
    }

    public byte get() {
        return hb[ix(nextGetIndex())];
    }

    public byte get(int i) {
        return hb[ix(checkIndex(i))];
    }


    public ByteBuffer get(byte[] dst, int offset, int length) {
        checkBounds(offset, length, dst.length);
        if (length > remaining())
            throw new BufferUnderflowException();
        System.arraycopy(hb, ix(position()), dst, offset, length);
        position(position() + length);
        return this;
    }

    public boolean isDirect() {
        return false;
    }


    public boolean isReadOnly() {
        return false;
    }

    public ByteBuffer put(byte x) {

        hb[ix(nextPutIndex())] = x;
        return this;


    }

    public ByteBuffer put(int i, byte x) {

        hb[ix(checkIndex(i))] = x;
        return this;


    }

    public ByteBuffer put(byte[] src, int offset, int length) {

        checkBounds(offset, length, src.length);
        if (length > remaining())
            throw new BufferOverflowException();
        System.arraycopy(src, offset, hb, ix(position()), length);
        position(position() + length);
        return this;


    }

    public ByteBuffer put(ByteBuffer src) {

        if (src instanceof java.nio.HeapByteBuffer) {
            if (src == this)
                throw new IllegalArgumentException();
            java.nio.HeapByteBuffer sb = (java.nio.HeapByteBuffer) src;
            int n = sb.remaining();
            if (n > remaining())
                throw new BufferOverflowException();
            System.arraycopy(sb.hb, sb.ix(sb.position()),
                hb, ix(position()), n);
            sb.position(sb.position() + n);
            position(position() + n);
        } else if (src.isDirect()) {
            int n = src.remaining();
            if (n > remaining())
                throw new BufferOverflowException();
            src.get(hb, ix(position()), n);
            position(position() + n);
        } else {
            super.put(src);
        }
        return this;


    }

    public ByteBuffer compact() {

        System.arraycopy(hb, ix(position()), hb, ix(0), remaining());
        position(remaining());
        limit(capacity());
        discardMark();
        return this;


    }


    public byte _get(int i) {                          // package-private
        return hb[i];
    }

    public void _put(int i, byte b) {                  // package-private

        hb[i] = b;


    }

    // char


    public char getChar() {
        return Bits.getChar(this, ix(nextGetIndex(2)), bigEndian);
    }

    public char getChar(int i) {
        return Bits.getChar(this, ix(checkIndex(i, 2)), bigEndian);
    }


    public ByteBuffer putChar(char x) {

        Bits.putChar(this, ix(nextPutIndex(2)), x, bigEndian);
        return this;


    }

    public ByteBuffer putChar(int i, char x) {

        Bits.putChar(this, ix(checkIndex(i, 2)), x, bigEndian);
        return this;


    }

    public CharBuffer asCharBuffer() {
        int size = this.remaining() >> 1;
        int off = offset + position();
        return (bigEndian
            ? (CharBuffer) (new ByteBufferAsCharBufferB(this,
            -1,
            0,
            size,
            size,
            off))
            : (CharBuffer) (new ByteBufferAsCharBufferL(this,
            -1,
            0,
            size,
            size,
            off)));
    }


    // short


    public short getShort() {
        return Bits.getShort(this, ix(nextGetIndex(2)), bigEndian);
    }

    public short getShort(int i) {
        return Bits.getShort(this, ix(checkIndex(i, 2)), bigEndian);
    }


    public ByteBuffer putShort(short x) {

        Bits.putShort(this, ix(nextPutIndex(2)), x, bigEndian);
        return this;


    }

    public ByteBuffer putShort(int i, short x) {

        Bits.putShort(this, ix(checkIndex(i, 2)), x, bigEndian);
        return this;


    }

    public ShortBuffer asShortBuffer() {
        int size = this.remaining() >> 1;
        int off = offset + position();
        return (bigEndian
            ? (ShortBuffer) (new ByteBufferAsShortBufferB(this,
            -1,
            0,
            size,
            size,
            off))
            : (ShortBuffer) (new ByteBufferAsShortBufferL(this,
            -1,
            0,
            size,
            size,
            off)));
    }


    // int


    public int getInt() {
        return Bits.getInt(this, ix(nextGetIndex(4)), bigEndian);
    }

    public int getInt(int i) {
        return Bits.getInt(this, ix(checkIndex(i, 4)), bigEndian);
    }


    public ByteBuffer putInt(int x) {

        Bits.putInt(this, ix(nextPutIndex(4)), x, bigEndian);
        return this;


    }

    public ByteBuffer putInt(int i, int x) {

        Bits.putInt(this, ix(checkIndex(i, 4)), x, bigEndian);
        return this;


    }

    public IntBuffer asIntBuffer() {
        int size = this.remaining() >> 2;
        int off = offset + position();
        return (bigEndian
            ? (IntBuffer) (new ByteBufferAsIntBufferB(this,
            -1,
            0,
            size,
            size,
            off))
            : (IntBuffer) (new ByteBufferAsIntBufferL(this,
            -1,
            0,
            size,
            size,
            off)));
    }


    // long


    public long getLong() {
        return Bits.getLong(this, ix(nextGetIndex(8)), bigEndian);
    }

    public long getLong(int i) {
        return Bits.getLong(this, ix(checkIndex(i, 8)), bigEndian);
    }


    public ByteBuffer putLong(long x) {

        Bits.putLong(this, ix(nextPutIndex(8)), x, bigEndian);
        return this;


    }

    public ByteBuffer putLong(int i, long x) {

        Bits.putLong(this, ix(checkIndex(i, 8)), x, bigEndian);
        return this;


    }

    public LongBuffer asLongBuffer() {
        int size = this.remaining() >> 3;
        int off = offset + position();
        return (bigEndian
            ? (LongBuffer) (new ByteBufferAsLongBufferB(this,
            -1,
            0,
            size,
            size,
            off))
            : (LongBuffer) (new ByteBufferAsLongBufferL(this,
            -1,
            0,
            size,
            size,
            off)));
    }


    // float


    public float getFloat() {
        return Bits.getFloat(this, ix(nextGetIndex(4)), bigEndian);
    }

    public float getFloat(int i) {
        return Bits.getFloat(this, ix(checkIndex(i, 4)), bigEndian);
    }


    public ByteBuffer putFloat(float x) {

        Bits.putFloat(this, ix(nextPutIndex(4)), x, bigEndian);
        return this;


    }

    public ByteBuffer putFloat(int i, float x) {

        Bits.putFloat(this, ix(checkIndex(i, 4)), x, bigEndian);
        return this;


    }

    public FloatBuffer asFloatBuffer() {
        int size = this.remaining() >> 2;
        int off = offset + position();
        return (bigEndian
            ? (FloatBuffer) (new ByteBufferAsFloatBufferB(this,
            -1,
            0,
            size,
            size,
            off))
            : (FloatBuffer) (new ByteBufferAsFloatBufferL(this,
            -1,
            0,
            size,
            size,
            off)));
    }


    // double


    public double getDouble() {
        return Bits.getDouble(this, ix(nextGetIndex(8)), bigEndian);
    }

    public double getDouble(int i) {
        return Bits.getDouble(this, ix(checkIndex(i, 8)), bigEndian);
    }


    public ByteBuffer putDouble(double x) {

        Bits.putDouble(this, ix(nextPutIndex(8)), x, bigEndian);
        return this;


    }

    public ByteBuffer putDouble(int i, double x) {

        Bits.putDouble(this, ix(checkIndex(i, 8)), x, bigEndian);
        return this;


    }

    public DoubleBuffer asDoubleBuffer() {
        int size = this.remaining() >> 3;
        int off = offset + position();
        return (bigEndian
            ? (DoubleBuffer) (new ByteBufferAsDoubleBufferB(this,
            -1,
            0,
            size,
            size,
            off))
            : (DoubleBuffer) (new ByteBufferAsDoubleBufferL(this,
            -1,
            0,
            size,
            size,
            off)));
    }


}
