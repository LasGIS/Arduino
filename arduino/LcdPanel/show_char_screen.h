#ifndef SHOWCHARSCREEN_H
#define SHOWCHARSCREEN_H

#include "lcd_screen.h"

class ShowCharScreen : public LcdScreen {
public:
//-----------------------
  // начальная страница для показа раскладки символов (0-15)
  int charsRow = 0;
//-----------------------
  ShowCharScreen();
  void showOnce();
  void edit(char key) {}
  void control(char key);
};

#endif // SHOWCHARSCREEN_H
