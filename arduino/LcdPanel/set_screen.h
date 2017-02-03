#ifndef SETSCREEN_H
#define SETSCREEN_H

#include "lcd_screen.h"

class SetScreen : public LcdScreen {
public:
  // --- данные ---
  uint8_t offset = 0;
  uint8_t editRow = 0;
  uint8_t commonMaxFields;
  LcdField * commonFields;
  uint8_t alarmMaxFields;
  LcdField * alarmFields;
  // --- методы ---
  SetScreen();
  void showOnce();
  void edit(char key);
  void control(char key);
  void hasBeyond(char key);
  void save();
  void load();
};

#endif // SETSCREEN_H
