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
  show,
  edit
};

struct FieldTftStruct {
  uint8_t row;
  uint8_t col;
  uint8_t fontSize;
  uint8_t len;
  uint16_t minVal;
  uint16_t maxVal;
  uint16_t val;
};

class FieldTft {
public:
  // --- данные ---
  uint8_t row;
  uint8_t col;
  uint8_t fontSize;
  uint8_t len;
  uint16_t minVal;
  uint16_t maxVal;
  uint16_t val;
  const char* (*getValue)(const int16_t val);
  // --- методы ---
  void setValue(int8_t nPosit, char key);
  void showField(int8_t nPosit = -1);
};

struct ScreenTftStruct {
  char* name;
  int16_t fieldsLength;
  FieldTftStruct** fields;
};

class ScreenTft {
public:
  // --- данные ---
  char* name;
  uint8_t nField;
  int8_t nPosit;
  int16_t fieldsLength;
  FieldTft* fields;
  // --- методы ---
  ScreenTft();
  ScreenTft(int16_t address);
  /** меняется ориентация */
  virtual void changeOrientation();
  /** показываем один раз при загрузке */
  virtual void showOnce();
  /** показываем раз в секунду */
  virtual void showTime(DateTime* dateTime);
  /** показываем часто ~10 мс */
  virtual void showEveryTime();
  /** работа с экраном в режиме редактирования */
  virtual void edit(char key);
  /** работа в обычном режиме */
  virtual void control(char key);

  /** показать текущее поле */
  void showCurrentField();
  /** показать все поля */
  void showAllFields();
};

#endif  // SCREEN_TFT_H
