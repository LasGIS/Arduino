/*
 *  @(#)ImmutableObject.java  last: 16.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.im;

import lombok.Getter;

import java.util.List;

/**
 * The Class ImmutableObject definition.
 *
 * @author Vladimir Laskin
 * @since 15.04.2023 : 20:18
 */
@Getter /* or realize Getters */
public final class ImmutableObject {
    private final String name;
    private final List<String> roles;
    public ImmutableObject(final String name, final String[] roles) {
        this.name = name;
        /* created {@link java.util.ImmutableCollections} */
        this.roles = List.of(roles);
    }
    public String getName() {
        return name;
    }
    public List<String> getRoles() {
        return roles;
    }
}
