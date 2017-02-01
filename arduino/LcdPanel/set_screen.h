#ifndef SETSCREEN_H
#define SETSCREEN_H

#include "lcd_screen.h"

class SetScreen : public LcdScreen {
public:
  SetScreen();
//  void showEveryTime();
  void showOnce();
  void edit(char key);
  void control(char key);
};

#endif // SETSCREEN_H
