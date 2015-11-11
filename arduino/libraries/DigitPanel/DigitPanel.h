#ifndef DigitPanel_h
#define DigitPanel_h

#include <MsTimer2.h>
#include <Arduino.h>

class DigitPanel {

private:
  //Пин подключен к ST_CP входу 74HC595
  int latchPin = 8;
  //Пин подключен к SH_CP входу 74HC595
  int clockPin = 12;
  //Пин подключен к DS входу 74HC595
  int dataPin = 11;
  // 4 Пина для управления цифрами
  int digit[4] = {10,9,7,4};


  int count = -1;
  long startTime = 0;
  static DigitPanel* _testActiveObject;

public:
  DigitPanel(int _latchPin, int _clockPin, int _dataPin, int* _digits, int _count);
  void start();
  void stop();

private:
  static inline void handle_interrupt();
  void interrupt();
  void print();
};

#endif
