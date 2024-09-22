/*
 *  @(#)FontDataPerformed.java  last: 22.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.data;

import lombok.Getter;
import lombok.extern.slf4j.Slf4j;

import java.io.File;
import java.util.LinkedList;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_C_FILE;
import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_C_SOURCE;
import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_H_FILE;
import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_H_SOURCE;

/**
 * The Class FontDataChangePerformed definition.
 *
 * @author Vladimir Laskin
 * @since 21.09.2024 : 12:41
 */
@Slf4j
public final class FontDataPerformed {
    private static final FontDataPerformed PERFORMED = new FontDataPerformed();
    /**
     * @deprecated todo: временно надо будет удалить
     */
    @Getter
    @Deprecated
    private static final FontData fontData = FontData.of();

    private final ScheduledExecutorService service;
    public LinkedList<FontDataListener> listeners = new LinkedList<>();
    public BlockingQueue<FontDataEventKey> queue = new LinkedBlockingQueue<>();

    public FontDataPerformed() {
        service = Executors.newSingleThreadScheduledExecutor();
        service.scheduleWithFixedDelay(this::perform, 0, 1, TimeUnit.SECONDS);
    }

    public static void stop(final long timeout, final TimeUnit unit) {
        PERFORMED.threadStop(timeout, unit);
    }

    public static void addListener(final FontDataListener listener) {
        log.trace("addListener({})", listener);
        PERFORMED.listeners.add(listener);
    }

    private static void addEvent(FontDataEventKey key) {
        PERFORMED.queue.add(key);
    }

    public static void setCFile(final File file) {
        fontData.setCFile(file);
        addEvent(CHANGE_C_FILE);
    }

    public static void setHFile(final File file) {
        fontData.setHFile(file);
        addEvent(CHANGE_H_FILE);
    }

    public static void setCSource(final StringBuilder cSource) {
        fontData.setCSource(cSource);
        addEvent(CHANGE_C_SOURCE);
    }

    public static void setHSource(final StringBuilder hSource) {
        fontData.setHSource(hSource);
        addEvent(CHANGE_H_SOURCE);
    }

    private void perform() {
        try {
            FontDataEventKey key = queue.take();
            log.trace("FontDataEventKey key = {}, listeners({})", key, listeners.size());
            switch (key) {
                case CHANGE_C_FILE:
                    log.trace("CHANGE_C_FILE");
                    listeners.forEach(listener -> listener.onChangeCFile(fontData.getCFile()));
                    break;
                case CHANGE_H_FILE:
                    log.trace("CHANGE_H_FILE");
                    listeners.forEach(listener -> listener.onChangeHFile(fontData.getHFile()));
                    break;
                case CHANGE_C_SOURCE:
                    log.trace("CHANGE_C_SOURCE");
                    listeners.forEach(listener -> listener.onChangeCSource(fontData.getCSource()));
                    break;
                case CHANGE_H_SOURCE:
                    log.trace("CHANGE_H_SOURCE");
                    listeners.forEach(listener -> listener.onChangeHSource(fontData.getHSource()));
                    break;
            }

        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

    private void threadStop(final long timeout, final TimeUnit unit) {
        try {
            log.info("threadStop({})", service.awaitTermination(timeout, unit));
            log.info("service.shutdownNow()");
            service.shutdownNow();
        } catch (final InterruptedException ex) {
            log.error("InterruptedException", ex);
            service.shutdownNow();
        }
    }
}
