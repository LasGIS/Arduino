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

  String panelValue;
  static DigitPanel* _activeDigitPanelObject;
  static volatile byte panel[4];

public:
  DigitPanel(int _latchPin, int _clockPin, int _dataPin, int* _digits, int _count);
  void setValue(String value);
  String getValue();

private:
  static inline void handle_interrupt();
  void showSegment();
  void resetAllDigit();
  byte getChar(char chr, boolean isPnt);
  void setNumBit(uint8_t mask);
};

#endif
