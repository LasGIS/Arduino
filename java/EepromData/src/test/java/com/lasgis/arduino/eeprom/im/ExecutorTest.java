/*
 *  @(#)ExecutorTest.java  last: 18.04.2023
 *
 * Title: LG Java for Arduino
 * Description: Program for support Arduino.
 * Copyright (c) 2023, LasGIS Company. All Rights Reserved.
 */

package com.lasgis.arduino.eeprom.im;

import lombok.extern.slf4j.Slf4j;
import org.junit.jupiter.api.Test;

import java.util.concurrent.Executor;

/**
 * <description>
 *
 * @author vlaskin
 * @since <pre>18.04.2023</pre>
 */
@Slf4j
public class ExecutorTest {
    private static final class ThreadExecutor implements Executor {
        Thread thread = null;

        @Override
        public void execute(final Runnable command) {
            thread = new Thread(command);
            thread.start();
        }

        public boolean isAlive() {
            return thread != null && thread.isAlive();
        }
    }

    public static final class MyCallable implements Runnable {
        @Override
        public void run() {
            for (int i = 0; i < 100; i++) {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                System.out.println(i);
            }
        }
    }

    @Test
    void executorTest() throws InterruptedException {
        final ThreadExecutor executor = new ThreadExecutor();
        executor.execute(new MyCallable());
        do {
            Thread.sleep(100);
            System.out.print(" check ");
        } while (executor.isAlive());
        System.out.println("finish");
    }
}
