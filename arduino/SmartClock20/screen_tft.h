#ifndef SCREEN_TFT_H
#define SCREEN_TFT_H

#include <stdint.h>
#include <DS3231.h>

#define ORIENTATION_HORISONTAL 0x01

enum OrientationType : uint8_t {
  undefine = 0xfe,
  bottom = 0,
  right = 1,
  top = 2,
  left = 3
};

enum ModeType : uint8_t {
  show, edit
};

class FieldTft {
public:
// --- ������ ---
  uint8_t row;
  uint8_t col;
  uint8_t fontSize;
  uint8_t len;
  uint16_t minVal;
  uint16_t maxVal;
  uint16_t val;
  const char* (*getValue) (const uint16_t val);
// --- ������ ---
  void setValue(int8_t nPosit, char key);
  void showField(int8_t nPosit = -1);
};

class ScreenTft {
public:
// --- ������ ---
  char * name;
  uint8_t maxFields;
  uint8_t nField;
  int8_t nPosit;
  FieldTft * fields;
// --- ������ ---
  ScreenTft();
  /** �������� ���������� */
  virtual void changeOrientation();
  /** ���������� ���� ��� ��� �������� */
  virtual void showOnce();
  /** ���������� ��� � ������� */
  virtual void showTime(DateTime * dateTime);
  /** ���������� ����� ~10 �� */
  virtual void showEveryTime();
  /** ������ � ������� � ������ �������������� */
  virtual void edit(char key);
  /** ������ � ������� ������ */
  virtual void control(char key);

  /** �������� ������� ���� */
  void showCurrentField();
  /** �������� ��� ���� */
  void showAllFields();
};

#endif // SCREEN_TFT_H
