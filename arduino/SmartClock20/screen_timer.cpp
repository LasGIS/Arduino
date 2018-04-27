#include "SmartClock20.h"

ScreenTimer::ScreenTimer(): ScreenTft() {
  name = (char*) "Timer ";
  maxFields = 2;
  fields = new FieldTft[maxFields + 1];
  fields[0] = {0, 12, 2, 0, 23, 0, NULL};         // ���
  fields[1] = {0, 15, 2, 0, 59, 0, NULL};         // ������
  fields[2] = {0, 18, 2, 0, 59, 0, NULL};         // �������
}

void ScreenTimer::showTime(DateTime * dateTime) {
  ScreenTft::showTime(dateTime);
}

void ScreenTimer::showEveryTime() {
}

void ScreenTimer::showOnce() {
  ScreenTft::showOnce();
}

/**
 * ��������������
 */
void ScreenTimer::edit(char key) {
  switch(key) {
  case 1: // ���������
    nField = 0;
    nPosit = 0;
    load();
    break;
  case 'M': // ����������
    save();
    break;
  }
  ScreenTft::edit(key);
}

/*void ScreenTimer::control(char key) {
  ScreenTft::control(key);
}*/

void ScreenTimer::load() {
  fields[0].val = Clock.getHour(h12, PM);
  fields[1].val = Clock.getMinute();
  fields[2].val = Clock.getSecond();
}

void ScreenTimer::save() {

}
