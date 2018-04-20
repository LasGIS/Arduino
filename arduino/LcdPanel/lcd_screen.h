#ifndef LCDSCREEN_H
#define LCDSCREEN_H

#include <stdint.h>

enum ModeType : uint8_t {
  show, edit
};

class LcdField {
public:
// --- ������ ---
  uint8_t row;
  uint8_t col;
  uint8_t len;
  uint16_t minVal;
  uint16_t maxVal;
  uint16_t val;
  const char* (*getValue) (const uint16_t val);
// --- ������ ---
  void setValue(int nPosit, char key);
  void showField(int nPosit);
};

class LcdScreen {
public:
// --- ������ ---
  uint8_t maxFields;
  uint8_t nField;
  int8_t nPosit;
  LcdField * fields;
// --- ������ ---
  LcdScreen();
  /** ���������� ������ 100 �� */
  virtual void showEveryTime();
  /** ���������� ���� ��� */
  virtual void showOnce();
  virtual void edit(char key);
  virtual void control(char key) {}
  /** ����� �� ������� ����������� ������� �������������� */
  virtual void hasBeyond(char key) {}
};

#endif // LCDSCREEN_H
