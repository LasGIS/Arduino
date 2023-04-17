/*
 *  @(#)IMTest.java  last: 17.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.im;

import lombok.extern.slf4j.Slf4j;
import org.junit.jupiter.api.Test;

import java.lang.annotation.Annotation;
import java.lang.reflect.Field;
import java.util.HashMap;
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
     * Задача:  List<Integer> list = List.of(1, 2, 3, 4, 5, 6, 7, 8, 9, 10); отфильтровать список и привести отфильтрованные объекты к типа Y и Z.
     * Есть метод для фильтрации (callExtSystem) и методы для преобразования (change2Y и change2Z)
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

    @Test
    void boxTest() throws Exception {
        Box<Integer> boxInteger = new Box<>(123);
//        Box<Integer> boxInt = new Box<>("String");
        Box<String> boxString = new Box<>("String");
        Annotation an = Box.class.getAnnotation(MyAnnotation.class);
        log.info("Annotation an = {}", an);
        Field item = Box.class.getDeclaredField("item");
        log.info("Field item = {}", item);
        Annotation anField = item.getAnnotation(MyAnnotation.class);
        log.info("Annotation anField = {}", anField);
    }

    @Test
    void testLinkedHashMap() {
        final HashMap<Integer, String> map = new HashMap<>();
        map.put(0, "02");
        map.put(1, "01");
        map.put(3, "03");
        map.put(4, "05");
        map.put(5, "06");
        map.put(2, "04");
        map.put(6, "07");
        map.put(7, "08");
        map.put(8, "09");
        map.put(9, "10");
        log.info("map = {}", map);
        List<String> list = map.values().parallelStream().sorted().collect(Collectors.toList());
        log.info("map = {}", map);
        log.info("list = {}", list);
        map.keySet().stream()
            .peek(number -> System.out.println("Filter operation for number: " + number)) //Peek is used for debugging purposes.
            .filter(number -> number % 2 == 1) //Filter the odd numbers
            .limit(4) //After 4th element stop the stream. Process first 4 element.
            .peek(number -> System.out.println("Map operation for number: " + number)) //Peek is used for debugging purposes.
            .map(number -> number * 2) //Transform the number to number*2
            .forEach(number -> System.out.println("Result of stream: " + number)); //Print the results.
        // ObjectOutputStream
    }
}
