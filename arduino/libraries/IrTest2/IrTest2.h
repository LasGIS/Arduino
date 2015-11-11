#ifndef IrTest2_h
#define IrTest2_h

#include <Arduino.h>

#define IR_TEST_READ_BUF_SIZE 700

class IrTest2 {

private:
  int irPin;
  short readBuf[IR_TEST_READ_BUF_SIZE];
  static IrTest2* _test2ActiveObject;

public:
  IrTest2(int pin);
  bool hasCode();
  unsigned long getCode();
  void start();
  void stop();

private:
  static inline void handle_interrupt();
  void event();
};

#endif
