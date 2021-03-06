#ifndef ALARMCLOCK_H
#define ALARMCLOCK_H

#include <DS1302.h>

class AlarmClock {
public:
  // --- ������ ---
  uint8_t number;
  uint8_t active      : 1;
  uint8_t isMonday    : 1;
  uint8_t isTuesday   : 1;
  uint8_t isWednesday : 1;
  uint8_t isThursday  : 1;
  uint8_t isFriday    : 1;
  uint8_t isSaturday  : 1;
  uint8_t isSunday    : 1;
  uint8_t min;
  uint8_t hour;
// --- ������ ---
  AlarmClock(uint8_t number);
  void toString(char* str, uint16_t size);
  void eepromSave();
  void eepromLoad(uint8_t _number);
  bool equal(Time time);
};

#endif // ALARMCLOCK_H
