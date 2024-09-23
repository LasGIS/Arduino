/*
 *  @(#)FontDataPerformed.java  last: 24.09.2024
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2024, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.editfont.data;

import lombok.extern.slf4j.Slf4j;

import java.io.File;
import java.util.LinkedList;
import java.util.Map;
import java.util.Optional;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import java.util.function.BiFunction;

import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_BASE_LINE;
import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_CHAR_HEIGHT;
import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_C_FILE;
import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_C_SOURCE;
import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_DATA_SIZE;
import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_FIRST_CHAR;
import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_H_FILE;
import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_H_SOURCE;
import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_NUMBER_CHARS;
import static com.lasgis.arduino.editfont.data.FontDataEventKey.CHANGE_WIDTH_CHAR;

/**
 * The Class FontDataChangePerformed definition.
 *
 * @author Vladimir Laskin
 * @since 21.09.2024 : 12:41
 */
@Slf4j
public final class FontDataPerformed {
    private static final FontDataPerformed PERFORMED = new FontDataPerformed();
    private static final Map<FontDataEventKey, BiFunction<FontDataListener, FontData, Boolean>>
        FONT_DATA_KEY_TO_PERFORM_MAP = Map.ofEntries(
        Map.entry(CHANGE_C_FILE, (listener, fontData) -> listener.onChangeCFile(fontData.getCFile())),
        Map.entry(CHANGE_H_FILE, (listener, fontData) -> listener.onChangeHFile(fontData.getHFile())),
        Map.entry(CHANGE_C_SOURCE, (listener, fontData) -> listener.onChangeCSource(fontData.getCSource())),
        Map.entry(CHANGE_H_SOURCE, (listener, fontData) -> listener.onChangeHSource(fontData.getHSource())),
        Map.entry(CHANGE_WIDTH_CHAR, (listener, fontData) -> listener.onChangeWidthChar(Optional.ofNullable(fontData.getWidthChar()).orElse(0))),
        Map.entry(CHANGE_NUMBER_CHARS, (listener, fontData) -> listener.onChangeNumberChars(Optional.ofNullable(fontData.getNumberChars()).orElse(0))),
        Map.entry(CHANGE_CHAR_HEIGHT, (listener, fontData) -> listener.onChangeCharHeight(Optional.ofNullable(fontData.getCharHeight()).orElse(0))),
        Map.entry(CHANGE_BASE_LINE, (listener, fontData) -> listener.onChangeBaseLine(Optional.ofNullable(fontData.getBaseLine()).orElse(0))),
        Map.entry(CHANGE_DATA_SIZE, (listener, fontData) -> listener.onChangeDataSize(Optional.ofNullable(fontData.getDataSize()).orElse(0))),
        Map.entry(CHANGE_FIRST_CHAR, (listener, fontData) -> listener.onChangeFirstChar(Optional.ofNullable(fontData.getFirstChar()).orElse(0)))
    );

    /**
     * Использование напрямую запрещено!
     *
     * @deprecated todo: надо будет удалить @Getter
     */
//    @Getter
    private static final FontData fontData = FontData.of();
    private static final BlockingQueue<FontDataEventKey> PERFORMED_QUEUE = new LinkedBlockingQueue<>();

    private final ScheduledExecutorService service;
    public LinkedList<FontDataListener> listeners = new LinkedList<>();

    public FontDataPerformed() {
        service = Executors.newSingleThreadScheduledExecutor();
        service.scheduleWithFixedDelay(this::perform, 0, 1, TimeUnit.MILLISECONDS);
    }

    public static void stop(final long timeout, final TimeUnit unit) {
        PERFORMED.threadStop(timeout, unit);
    }

    public static void addListener(final FontDataListener listener) {
        addListener(listener, false);
    }

    public static void addListener(final FontDataListener listener, final boolean withPerform) {
        log.trace("addListener({})", listener);
        if (withPerform) {
            for (FontDataEventKey key : FontDataEventKey.values()) {
                log.trace("Perform {} = {}", key.name(), FONT_DATA_KEY_TO_PERFORM_MAP.get(key).apply(listener, fontData));
            }
        }
        PERFORMED.listeners.add(listener);
    }

    private static void addEvent(FontDataEventKey key) {
        PERFORMED_QUEUE.add(key);
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

    public static void setWidthChar(final Integer value) {
        fontData.setWidthChar(value);
        addEvent(CHANGE_WIDTH_CHAR);
    }

    public static void setNumberChars(final Integer value) {
        fontData.setNumberChars(value);
        addEvent(CHANGE_NUMBER_CHARS);
    }

    public static void setCharHeight(final Integer value) {
        fontData.setCharHeight(value);
        addEvent(CHANGE_CHAR_HEIGHT);
    }

    public static void setBaseLine(final Integer value) {
        fontData.setBaseLine(value);
        addEvent(CHANGE_BASE_LINE);
    }

    public static void setDataSize(final Integer value) {
        fontData.setDataSize(value);
        addEvent(CHANGE_DATA_SIZE);
    }

    public static void setFirstChar(final Integer value) {
        fontData.setFirstChar(value);
        addEvent(CHANGE_FIRST_CHAR);
    }

    /**
     * Производим перерасчет.
     */
    private void perform() {
        try {
            final FontDataEventKey key = PERFORMED_QUEUE.take();
            log.trace("FontDataEventKey key = {}, listeners({})", key, listeners.size());
            listeners.forEach(listener -> FONT_DATA_KEY_TO_PERFORM_MAP.get(key).apply(listener, fontData));
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
