/*
 *  @(#)FontDataAdapter.java  last: 22.09.2024
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
    public void onChangeCFile(final File file) {
    }

    @Override
    public void onChangeHFile(final File file) {
    }

    @Override
    public void onChangeCSource(final StringBuilder stringBuilder) {
    }

    @Override
    public void onChangeHSource(final StringBuilder stringBuilder) {
    }

    @Override
    public void onChangeWidthChar(Integer value) {
    }

    @Override
    public void onChangeNumberChars(Integer value) {
    }

    @Override
    public void onChangeCharHeight(Integer value) {
    }

    @Override
    public void onChangeBaseLine(Integer value) {
    }

    @Override
    public void onChangeDataSize(Integer value) {
    }

    @Override
    public void onChangeFirstChar(Integer value) {
    }
}
