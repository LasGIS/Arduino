/*
 *  @(#)FontDataAdapter.java  last: 23.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.data;

import java.io.File;

/**
 * The Class FontDataChangeAdapter definition.
 *
 * @author Vladimir Laskin
 * @since 21.09.2024 : 12:15
 */
public abstract class FontDataAdapter implements FontDataListener {
    @Override
    public boolean onChangeCFile(final File file) {
        return false;
    }

    @Override
    public boolean onChangeHFile(final File file) {
        return false;
    }

    @Override
    public boolean onChangeCSource(final StringBuilder stringBuilder) {
        return false;
    }

    @Override
    public boolean onChangeHSource(final StringBuilder stringBuilder) {
        return false;
    }

    @Override
    public boolean onChangeWidthChar(Integer value) {
        return false;
    }

    @Override
    public boolean onChangeNumberChars(Integer value) {
        return false;
    }

    @Override
    public boolean onChangeCharHeight(Integer value) {
        return false;
    }

    @Override
    public boolean onChangeBaseLine(Integer value) {
        return false;
    }

    @Override
    public boolean onChangeDataSize(Integer value) {
        return false;
    }

    @Override
    public boolean onChangeFirstChar(Integer value) {
        return false;
    }
}
