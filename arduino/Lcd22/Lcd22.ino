#include <Arduino.h>
#include <avr/pgmspace.h>
#include <TFTv2.h>
#include <SPI.h>
#include "Lcd22.h"

#define MEASURIES_SIZE 6
#define FONT_SIZE 1
#define CHAR_WIDTH FONT_SPACE * FONT_SIZE
#define CHAR_HEIGHT FONT_Y * FONT_SIZE

LgMeasure::LgMeasure(const char* _description,
    uint8_t _pin, uint8_t _decimal,
    uint16_t _color,
    float _factor,
    MeasureType _typeOut,
    bool _isGraph
) {
    char* buffer = new char[strlen_P(_description) + 2];
    strcpy_P(buffer, _description);
    description = buffer;
    pin = _pin;
    decimal = _decimal;
    color = _color;
    factor = _factor;
    typeOut = _typeOut;
    isGraph = _isGraph;
}

const char prgm_str00[] PROGMEM = "нап Зарядка = ";
const char prgm_str01[] PROGMEM = "ток Батареи = ";
const char prgm_str02[] PROGMEM = "нап Батарея = ";
const char prgm_str03[] PROGMEM = "ток  +5     = ";
const char prgm_str04[] PROGMEM = "напр +5     = ";
const char prgm_str05[] PROGMEM = "напр +3.3   = ";

const LgMeasure* measuries[] = {
  /* нап Зарядка */
  new LgMeasure(prgm_str00, A0, 2, CHARGER_VT_COLOR, 0.01175, VOLTAGE, true),
  /* нап Батарея */
  new LgMeasure(prgm_str02, A2, 2, BATTERY_VT_COLOR, 0.00630, VOLTAGE, true),
  /* напр +5 */
  new LgMeasure(prgm_str04, A6, 2, SUPPLY5_VT_COLOR, 0.01175, VOLTAGE, true),

  /* напр +3.3 */
  new LgMeasure(prgm_str05, A7, 2, SUPPLY3_VT_COLOR, 0.00630, VOLTAGE, true),
  /* ток Батареи */
  new LgMeasure(prgm_str01, A1, 3, BATTERY_IT_COLOR, 0.00106818, CURRENT, true),
  /* ток  +5 */
  new LgMeasure(prgm_str03, A3, 3, SUPPLY5_IT_COLOR, 0.0007477, CURRENT, true),
};

int curX = screenLeft - 1;
int curNextX = curX + 1;
char comBuffer[20];

void setup() {
    analogReference(INTERNAL);
    Serial.begin(9600);
    TFT_BL_ON;        // turn on the background light
    Tft.TFTinit();    //init TFT library
    drawFirst();
    drawGrid();
}

void loop() {
  measuring();
  delay(500);
}

/**
 * Показываем первоначальную текстовку
 */
void drawFirst(){
  for (int i = 0; i < MEASURIES_SIZE; i++) {
    int col = i / 3;
    int pos = i % 3;
    Tft.drawString(
      measuries[i]->description,
      CHAR_WIDTH * col * 20, CHAR_HEIGHT * pos,
      FONT_SIZE, WHITE
    );
  }
}

int calcY(int ty) {
  ty -= 4;
  return (ty < screenTop) ? screenTop
    : ((ty > screenBottom - 6) ? screenBottom - 6 : ty);
}

inline int calcX(int time, int tx) {
  return time < 10 ? tx - 2 : tx - 6;
  //return time < 10 ? (time == 0 ? tx : tx - 2) : tx - 6;
}

/**
 * показываем решётку
 */
void drawGrid() {
  Tft.drawRectangle(screenLeft, screenTop, screenHSize, screenVSize, markHourColor);
  for (float temp = CURRENT_START;
    temp <= CURRENT_START + screenVSize / CURRENT_MULTIPLIER;
    temp += CURRENT_DELTA
  ) {
    int ty = (int) (screenBottom - (temp - CURRENT_START) * CURRENT_MULTIPLIER);
    Tft.drawFloat(temp, 2, screenRigth - 7, calcY(ty),
      FONT_SIZE, BATTERY_IT_COLOR
    );
  }
  for (float volt = VOLTAGE_START;
    volt <= VOLTAGE_START + screenVSize / VOLTAGE_MULTIPLIER;
    volt += VOLTAGE_DELTA
  ) {
    int ty = (int) (screenBottom - (volt - VOLTAGE_START) * VOLTAGE_MULTIPLIER);
    Tft.drawFloat(volt, 1, 0, calcY(ty),
      FONT_SIZE, voltColor
    );
  }

#if defined (TIME_TYPE_6_MIN)
  /* весь экран - 6 мин */
  for (int time = 0; time <= 6; time++) {
    int tx = (int) (screenLeft + time * 48);
    Tft.drawNumber(time, calcX(time, tx),
      screenBottom + 2, FONT_SIZE, colorTime
    );
  }
#elif defined (TIME_TYPE_60_MIN)
  // весь экран - 1 час
  for (int time = 0; time <= 6; time++) {
    int tx = (int) (screenLeft + time * 48);
    Tft.drawNumber(time * 10, calcX(time, tx),
      screenBottom + 2, FONT_SIZE, colorTime
    );
  }
#elif defined (TIME_TYPE_8_HOUR)
  // весь экран - 8 часов
  for (int time = 0; time <= 8; time++) {
    int tx = (int) (screenLeft + time * 36);
    Tft.drawNumber(time, calcX(time, tx),
      screenBottom + 2, FONT_SIZE, colorTime
    );
  }
#elif defined (TIME_TYPE_24_HOUR)
  // весь экран - 24 часа
  for (int time = 0; time <= 24; time += 3) {
    int tx = (int) (screenLeft + time * 12);
    Tft.drawNumber(time, calcX(time, tx),
      screenBottom + 2, FONT_SIZE, colorTime
    );
  }
#endif
}

/**
 * Показываем метки
 */
void showTempMarks() {
  for (float val = VOLTAGE_START;
       val <= VOLTAGE_START + screenVSize / VOLTAGE_MULTIPLIER;
       val += 0.25
  ) {
    Tft.setPixel(
      curX,
      (int) (screenBottom - (val - VOLTAGE_START) * VOLTAGE_MULTIPLIER),
      ((int) (val * 10) % 5 == 0) ? markHourColor : markMinColor
    );
  }
  Tft.setPixel(curX, screenBottom, markHourColor);
  Tft.setPixel(curX, screenTop, markHourColor);
}

/**
 * выводим время и дату в формате.
 */
void printTime(long time) {
  unsigned long milTime = time / 1000;
  int sec = milTime % 60;
  int min = (milTime / 60) % 60;
  int hour = milTime / 3600;
  snprintf(comBuffer, sizeof(comBuffer), "%d:%02d:%02d", hour, min, sec);
  Tft.fillRectangle(260, 0, 60, CHAR_HEIGHT, BLACK);
  Tft.drawString(comBuffer, 260, 0, FONT_SIZE, colorTime);
}

/**
 * @brief measury
 */
void measury() {
  for (int i = 0; i < MEASURIES_SIZE; i++) {
    int col = i / 3;
    int pos = i % 3;
    float val = analogRead(measuries[i]->pin) * measuries[i]->factor;

#ifdef HAS_SERIAL
    Serial.print(measuries[i]->description);
    Serial.println(val, 3);
#endif

    Tft.fillRectangle(
      CHAR_WIDTH * (col * 20 + 14), CHAR_HEIGHT * pos,
      CHAR_WIDTH * 6              , CHAR_HEIGHT, BLACK
    );
    Tft.drawFloat(
      val, measuries[i]->decimal,
      CHAR_WIDTH * (col * 20 + 14), CHAR_HEIGHT * pos, FONT_SIZE,
      measuries[i]->color
    );
    if (measuries[i]->isGraph) {
      int ty = screenBottom;
      switch (measuries[i]->typeOut) {
        case TEMPERATURE:
          ty = (int) (screenBottom - (val - TEMPERATURE_START) * TEMPERATURE_MULTIPLIER);
          break;
        case VOLTAGE:
          ty = (int) (screenBottom - (val - VOLTAGE_START) * VOLTAGE_MULTIPLIER);
          break;
        case CURRENT:
          ty = (int) (screenBottom - (val - CURRENT_START) * CURRENT_MULTIPLIER);
          break;
        default:
          break;
      }
      if (ty > screenTop && ty < screenBottom) {
        Tft.setPixel(curX, ty, measuries[i]->color);
      }
    }
  }
#ifdef HAS_SERIAL
  Serial.println(" ----------- ");
#endif
}

/**
 *
 */
void measuring() {

  long time = millis();
  printTime(time);
  long timeInval = time % TIME_FUL_SCREEN;
  int curXCalc = screenLeft + timeInval / TIME_MULTIPLIER;

  if (curX != curXCalc) {
    curX = curXCalc;
    if (curX >= screenRigth) curX = screenLeft;
    curNextX = curX + 1;
    if (curNextX >= screenRigth) curNextX = screenLeft;
    Tft.drawVerticalLine(curNextX, screenTop, screenVSize, WHITE);

    uint16_t color;
    // главная метка времени (3 часа)
    if (timeInval % TIME_MAIN_MARK < TIME_MULTIPLIER) {
      color = markHourColor;
    }
    // дополнительная метка времени (1 час)
    else if (timeInval % TIME_HALF_MARK < TIME_MULTIPLIER) {
      color = markMinColor;
    }
    else {
      color = foneColor;
    }
    Tft.drawVerticalLine(curX, screenTop + 1, screenVSize - 2, color);
    showTempMarks();

  }
  measury();
}
