/*
 *  @(#)FontDataListener.java  last: 22.09.2024
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
    void onChangeCFile(final File file);

    void onChangeHFile(final File file);

    void onChangeCSource(final StringBuilder stringBuilder);

    void onChangeHSource(final StringBuilder stringBuilder);
}
