#include "LcdPanel.h"
#include <EEPROM.h>
#include <DS1302.h>
#include "alarm_clock.h"

AlarmClock::AlarmClock(uint8_t _number) {
  eepromLoad(_number);
}

char toChar(uint8_t val) {
  return val ? 7 : '-';
}

void AlarmClock::toString(char *str, uint16_t size) {
  snprintf(str, size, "%d%c %c%c%c%c%c%c%c %02d:%02d",
    number + 1, toChar(active),
    toChar(isMonday),
    toChar(isTuesday),
    toChar(isWednesday),
    toChar(isThursday),
    toChar(isFriday),
    toChar(isSaturday),
    toChar(isSunday),
    hour, min
  );
}

void AlarmClock::eepromSave() {
  int start = ALARM_CLOCK_ADR + ALARM_CLOCK_ADR_SIZE * number;
  EEPROM.put(start, (AlarmClock&) (*this));
}
void AlarmClock::eepromLoad(uint8_t _number) {
  int start = ALARM_CLOCK_ADR + ALARM_CLOCK_ADR_SIZE * _number;
  EEPROM.get(start, (AlarmClock&) (*this));
  number = _number;
}

bool AlarmClock::equal(Time time) {
  if (active) {
    bool isEqual = false;
    bool isNotEmpty = isMonday || isTuesday || isWednesday || isThursday || isFriday || isSaturday || isSunday;
    if (isNotEmpty) {
      switch (time.day) {
      case Time::Day::kSunday:
        isEqual = isSunday;
        break;
      case Time::Day::kMonday:
        isEqual = isMonday;
        break;
      case Time::Day::kTuesday:
        isEqual = isTuesday;
        break;
      case Time::Day::kWednesday:
        isEqual = isWednesday;
        break;
      case Time::Day::kThursday:
        isEqual = isThursday;
        break;
      case Time::Day::kFriday:
        isEqual = isFriday;
        break;
      case Time::Day::kSaturday:
        isEqual = isSaturday;
        break;
      }
    } else {
      isEqual = true;
    }
    if (isEqual && time.hr == hour && time.min == min && time.sec < 10) {
      if (!isNotEmpty) {
        active = false;
        eepromSave();
      }
      return true;
    }
    return false;
  }
  return false;
}
