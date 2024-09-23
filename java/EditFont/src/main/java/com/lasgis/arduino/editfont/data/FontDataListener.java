/*
 *  @(#)FontDataListener.java  last: 23.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.data;

import java.io.File;
import java.util.EventListener;

/**
 * The Class FontDataChange definition.
 *
 * @author Vladimir Laskin
 * @since 21.09.2024 : 11:54
 */
public interface FontDataListener extends EventListener {
    boolean onChangeCFile(final File file);

    boolean onChangeHFile(final File file);

    boolean onChangeCSource(final StringBuilder stringBuilder);

    boolean onChangeHSource(final StringBuilder stringBuilder);

    boolean onChangeWidthChar(final Integer value);

    boolean onChangeNumberChars(final Integer value);

    boolean onChangeCharHeight(final Integer value);

    boolean onChangeBaseLine(final Integer value);

    boolean onChangeDataSize(final Integer value);

    boolean onChangeFirstChar(final Integer value);
}
