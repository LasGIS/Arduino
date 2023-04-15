/*
 *  @(#)IMTest.java  last: 15.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.im;

import lombok.extern.slf4j.Slf4j;
import org.junit.jupiter.api.Test;

import java.util.List;
import java.util.stream.Collectors;

/**
 * <description>
 *
 * @author vlaskin
 * @since <pre>15.04.2023</pre>
 */
@Slf4j
public class IMTest {

    final List<Integer> list = List.of(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

    private boolean callExtSystem(final Integer x) {
        try {
            Thread.sleep(100);
        } catch (InterruptedException ex) {
            log.error(ex.getMessage(), ex);
        }
        return (x % 2 == 0);
    }

    private Integer change2Y(final Integer x) {
        return x * 5;
    }

    private Integer change2Z(final Integer x) {
        return x * 2;
    }

    /**
     * Задача:  List<Integer> list = List.of(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
     * отфильтровать список и привести отфильтрованные объекты к типа Y и Z.
     * Есть метод для фильтрации (callExtSystem)
     * и методы для преобразования (change2Y и change2Z)
     */
    @Test
    void sortList() throws Exception {
        List<Integer> l1 = list.stream().filter(this::callExtSystem).map(this::change2Y).collect(Collectors.toList());
        List<Integer> l2 = list.stream().filter(this::callExtSystem).map(this::change2Z).collect(Collectors.toList());
        log.info("l1 = {}, l2 = {}", l1, l2);
    }

    @Test
    void sortList2() throws Exception {
        /*
         * -"классическое" решение. Оно рабочее. Но в нем дважды выполняется фильтрация. Чтобы избежать двойной фильтрации,
         * я реализовал решение по другому:
         */
        List<Integer> filtered = list.stream().filter(this::callExtSystem).collect(Collectors.toList());
        List<Integer> l1 = filtered.stream().map(this::change2Y).collect(Collectors.toList());
        List<Integer> l2 = filtered.stream().map(this::change2Z).collect(Collectors.toList());
        log.info("l1 = {}, l2 = {}", l1, l2);
        /*
         * Какие недостатки в моем новом решении (в последней строке будет ошибка времени выполнения, т.к.поток закрыт)
         */
    }
}
