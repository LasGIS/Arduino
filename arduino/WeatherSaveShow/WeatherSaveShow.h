#ifndef REALTIME_H
#define REALTIME_H

#include <TFT_lg.h>
#include <DS1302.h>

//#define HAS_SERIAL
#define TEMPERATURE_START 10.0
#define TEMPERATURE_MULTIPLIER 2.0
/* весь экран - 8 часов */
//#define TIME_MULTIPLIER 200000
/* весь экран - 24 часа */
#define TIME_MULTIPLIER 600000

#define backColor     0b0000000000000000
#define textColor     0b1111111111111111
#define colorT1       0b0000000111011111
#define colorT2       0b0111100111011111
#define colorB1       0b0000011111100000
#define colorB2       0b0111111111100000
#define colorTime     0b0111101111101111
#define editColorTime 0b1111100000000000
#define voltColor     0b1111100100000100
#define foneColor     0b0001100011100011
#define errorColor    0b0000000000011111
#define markColor     0b1111111111111111
#define markTempColor 0b1001010010010010
#define markMinColor  0b0110001100001100
#define markHourColor 0b1001010010010010

#define screenTop     19
#define screenBottom  119
#define screenLeft    0
#define screenRigth   144

class LcdField {
public:
  uint8_t row;
  uint8_t col;
  uint8_t len;
  uint16_t maxVal;
  uint16_t val;
};

enum ShowTimeType : uint8_t {
    DataTime,
    DataMode,
    TimeMode
};

char * printTime(Time * time, ShowTimeType showMode);
bool editTime(char key);

#endif // REALTIME_H
