#ifndef DigitPanel_h
#define DigitPanel_h

#include <MsTimer2.h>
#include <Arduino.h>

class DigitPanel {

private:
  //��� ��������� � ST_CP ����� 74HC595
  int latchPin = 8;
  //��� ��������� � SH_CP ����� 74HC595
  int clockPin = 12;
  //��� ��������� � DS ����� 74HC595
  int dataPin = 11;
  // 4 ���� ��� ���������� �������
  int digit[4] = {10,9,7,4};

  int place = 0;
  unsigned int chars = 0;
  int paused = 0;
  String panelValue;
  static DigitPanel* _activeDigitPanelObject;

public:
  DigitPanel(int _latchPin, int _clockPin, int _dataPin, int* _digits, int _count);
  void setValue(String value);

private:
  static inline void handle_interrupt();
  void showSegment();
  void resetAllDigit();
  void setChar(char chr, boolean isPnt);
  void setNumBit(uint8_t mask);
};

#endif
