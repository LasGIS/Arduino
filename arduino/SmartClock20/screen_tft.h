#ifndef SCREEN_TFT_H
#define SCREEN_TFT_H

#include <stdint.h>

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
// --- данные ---
  uint8_t row;
  uint8_t col;
  uint8_t len;
  uint16_t minVal;
  uint16_t maxVal;
  uint16_t val;
  const char* (*getValue) (const uint16_t val);
// --- методы ---
  void setValue(int nPosit, char key);
  void showField(int nPosit);
};

class ScreenTft {
public:
// --- данные ---
  char * name;
  uint8_t maxFields;
  uint8_t nField;
  int8_t nPosit;
  FieldTft * fields;
// --- методы ---
  ScreenTft();
  /** меняется ориентация */
  virtual void changeOrientation(OrientationType orientation);
  /** показываем один раз при загрузке */
  virtual void showOnce();
  /** показываем раз в секунду */
  virtual void showTime();
  /** показываем часто ~10 мс */
  virtual void showEveryTime();
  /** работа с экраном в режиме редактирования */
  virtual void edit(char key);
  /** работа в обычном режиме */
  virtual void control(char key);
};

#endif // SCREEN_TFT_H
