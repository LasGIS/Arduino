#include "SmartClock20.h"
#include "screen_timer.h"

extern ModeType mode;

ScreenTimer::ScreenTimer(): ScreenTft() {
  name = (char*) "Timer ";
  maxFields = 2;
  fields = new FieldTft[maxFields + 1];
  fields[0] = {1, 7,  2, 0, 23, 0, NULL};         // ���
  fields[1] = {1, 10, 2, 0, 59, 0, NULL};         // ������
  fields[2] = {1, 13, 2, 0, 59, 0, NULL};         // �������
}

void ScreenTimer::showEveryTime() {
  if (mode == show) {
  }
}

/**
 * ������� ����� � ���� �� LCD.
 */
void ScreenTimer::showOnce() {
  ScreenTft::showOnce();
}

/**
 * �������������� �������
 */
void ScreenTimer::edit(char key) {
  ScreenTft::edit(key);
}

void ScreenTimer::control(char key) {
  ScreenTft::control(key);
}
