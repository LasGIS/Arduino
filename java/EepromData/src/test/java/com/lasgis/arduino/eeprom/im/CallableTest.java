/*
 *  @(#)CallableTest.java  last: 19.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.im;

import lombok.extern.slf4j.Slf4j;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.util.Collection;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.Callable;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

/**
 * The Class CallableTest definition.
 *
 * @author Vladimir Laskin
 * @since 17.04.2023 : 20:55
 */
@Slf4j
public class CallableTest {
    private static final List<List<Integer>> SOURCE_LISTS = List.of(
        List.of(1, 3, 5, 7, 9),
        List.of(2, 4, 6, 8, 10),
        List.of(3, 22, 33, 44, 55),
        List.of(4, 2, 6, 1, 9, 10, 45)
    );
    private static final int NUM_THREADS = SOURCE_LISTS.size();
    private static final Integer REAL_SUM = SOURCE_LISTS.stream().flatMap(Collection::stream).reduce(0, Integer::sum);

    public static class MyCallable implements Callable<Integer> {
        private final List<Integer> list;

        public MyCallable(final List<Integer> list) {
            this.list = list;
        }

        @Override
        public Integer call() throws Exception {
            Integer acc = 0;
            for (Integer integer : list) {
                log.info("before {} ", integer);
                Thread.sleep(100);
                acc += integer;
                log.info("after {} ", integer);
            }
            return acc;
        }
    }

    @Test
    void callableTest() throws InterruptedException {
        final ExecutorService pool = Executors.newFixedThreadPool(NUM_THREADS);
        final Set<Future<Integer>> futures = new HashSet<>(NUM_THREADS);
        for (List<Integer> list : SOURCE_LISTS) {
            final Future<Integer> future = pool.submit(new MyCallable(list));
            futures.add(future);
        }
        if (pool.awaitTermination(10, TimeUnit.SECONDS)) {
            log.info("все выполнились");
        } else {
            log.info("закончилось время");
        }
//        do { Thread.sleep(10); } while (((ThreadPoolExecutor) pool).getActiveCount() > 0);
        final CompletableFuture<Integer> cFuture = CompletableFuture.supplyAsync(() -> {
            Integer sum = 0;
            boolean isDone;
            try {
                do {
                    isDone = true;
                    for (Future<Integer> future : futures) {
                        isDone = isDone && future.isDone();
                    }
                    Thread.sleep(10);
                    log.info("isDone = {}", isDone);
                } while (!isDone);
                for (Future<Integer> future : futures) {
                    sum += future.get();
                }
            } catch (InterruptedException | ExecutionException ex) {
                log.error(ex.getMessage(), ex);
                throw new RuntimeException(ex);
            }
            return sum;
        }).thenApplyAsync(sum -> {
            log.info("sum = {}, REAL SUM = {}", sum, REAL_SUM);
            Assertions.assertEquals(REAL_SUM, sum);
            return sum;
        });
        cFuture.join();
//        try {
//            cFuture.get();
//        } catch (InterruptedException | ExecutionException ex) {
//            log.error(ex.getMessage(), ex);
//            throw new RuntimeException(ex);
//        }
/*

        final Future<Integer> future = executor.submit(new SumCallable(futures));

        boolean isDone;
        try {
            synchronized (sinc) {
                do {
                    sinc.wait();
                    isDone = future.isDone();
                    log.info("Main isDone = {}", isDone);
                } while (!isDone);
            }
            int sum = future.get();
            log.info("sum = {}, REAL SUM = {}", sum, REAL_SUM);
            Assertions.assertEquals(REAL_SUM, sum);
        } catch (InterruptedException | ExecutionException ex) {
            log.error(ex.getMessage(), ex);
            throw new RuntimeException(ex);
        }
*/
    }
}
