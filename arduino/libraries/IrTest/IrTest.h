#ifndef IrTest_h
#define IrTest_h

#include <Arduino.h>

#define IR_TEST_MINIMAL_TIME 200
//#define IR_TEST_READ_BUF_SIZE 400
#define IR_TEST_INTERRUPT_BUF_SIZE 66

class IrTestPoint {
public:
  long time;
  byte val;
};

class IrTest {

private:
  int irPin;
  int count = -1;
  long startTime = 0;
  //short readBuf[IR_TEST_READ_BUF_SIZE];
  IrTestPoint points[IR_TEST_INTERRUPT_BUF_SIZE] ;
  static IrTest* _testActiveObject;

public:
  IrTest(int pin);
  void start();
  void stop();
  void print();

private:
  static inline void handle_interrupt();
  void interrupt();
};

#endif
