#include "LcdPanel.h"
#include "alarm_clock.h"

AlarmClock::AlarmClock(uint8_t _number) {
  number = _number;
  isMonday = 1;
  isTuesday = 1;
  isWednesday = 1;
  isThursday = 1;
  isFriday = 1;
}

char toChar(uint8_t val) {
  return val ? 7 : '-';
}

void AlarmClock::toString(char *str, uint16_t size) {
  snprintf(str, size, "%d%c %c%c%c%c%c%c%c %02d:%02d",
    number, toChar(active),
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
