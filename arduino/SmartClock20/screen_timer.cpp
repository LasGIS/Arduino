#include "SmartClock20.h"
#include "screen_timer.h"

extern LPModeType mode;

ScreenDateTime::ScreenDateTime(): ScreenTft() {
  maxFields = 2;
  fields = new FieldTft[maxFields + 1];
  fields[0] = {1, 7,  2, 0, 23, 0, NULL};         // ���
  fields[1] = {1, 10, 2, 0, 59, 0, NULL};         // ������
  fields[2] = {1, 13, 2, 0, 59, 0, NULL};         // �������
}

void ScreenDateTime::showEveryTime() {
  if (mode == show) {
  }
}

/**
 * ������� ����� � ���� �� LCD.
 */
void ScreenDateTime::showOnce() {
}

/**
 * �������������� �������
 */
void ScreenDateTime::edit(char key) {
  ScreenTft::edit(key);
}

void ScreenDateTime::control(char key) {
}
