#include "SmartClock20.h"

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_LG_ILI9225 tft(TFT_RST, TFT_RS, TFT_CS, TFT_LED);

// указываем пин для ИК датчика
IrControl irControl(2);

char comBuffer[20];
uint16_t X0, X1, Y0, Y1;
uint16_t ClockX0, ClockX1, ClockY0, ClockY1;
uint16_t clockX;
uint16_t clockY;
uint16_t boxCenterX;
uint16_t boxCenterY;

/**
 * @brief drawDouble
 * @param x
 * @param y
 * @param val
 * @param color
 */
void drawDouble(uint16_t x, uint16_t y, double val, uint16_t color) {
  //comBuffer[0] = val > 0 ? ' ' : '-';
  dtostrf(val, 5, 2, comBuffer);
  int len = strlen(comBuffer);
  comBuffer[len] = ' ';
  comBuffer[len + 1] = 0;
  tft.drawText(x, y, comBuffer, color);
}

/**
 * @brief setOrientation
 * Поправляем ориентацию в зависимости от показаний гравитационного датчика
 */
GravVector setOrientation(GravVector vec) {
  static uint8_t oldOrientation = -2;
  static uint8_t orientation = 2;
  if (vec.Y > GRAVI_FACTOR) {
    orientation = 2;
  } else if (vec.Y < -GRAVI_FACTOR) {
    orientation = 0;
  } else if (vec.X > GRAVI_FACTOR) {
    orientation = 1;
  } else if (vec.X < -GRAVI_FACTOR) {
    orientation = 3;
  }
#ifdef HAS_SERIAL
  Serial.println(orientation);
#endif
  switch (orientation) {
  default:
  case 2:
  case 0:
    X0 = BOXV_X0;
    X1 = BOXV_X1;
    Y0 = BOXV_Y0;
    Y1 = BOXV_Y1;
    clockX = CLOCKV_X;
    clockY = CLOCKV_Y;
    ClockX0 = BOXCLOCKV_X0;
    ClockX1 = BOXCLOCKV_X1;
    ClockY0 = BOXCLOCKV_Y0;
    ClockY1 = BOXCLOCKV_Y1;
    boxCenterX = BOXV_CENTER_X;
    boxCenterY = BOXV_CENTER_Y;
    break;
  case 1:
  case 3:
    X0 = BOXH_X0;
    X1 = BOXH_X1;
    Y0 = BOXH_Y0;
    Y1 = BOXH_Y1;
    clockX = CLOCKH_X;
    clockY = CLOCKH_Y;
    ClockX0 = BOXCLOCKH_X0;
    ClockX1 = BOXCLOCKH_X1;
    ClockY0 = BOXCLOCKH_Y0;
    ClockY1 = BOXCLOCKH_Y1;
    boxCenterX = BOXH_CENTER_X;
    boxCenterY = BOXH_CENTER_Y;
    break;
  }
  if (orientation != oldOrientation) {
    tft.clear();
    tft.setOrientation(orientation);
    tft.drawRectangle(X0, Y0, X1, Y1, COLOR_WHITE);
    tft.drawRectangle(ClockX0, ClockY0, ClockX1, ClockY1, COLOR_WHITE);
//    tft.fillRectangle(ClockX0 + 1, ClockY0 + 1, ClockX1 - 1, ClockY1 - 1, COLOR_GRAY);
  #ifdef ADXL345_ENABLED
    tft.drawText(0, 8, "X=", COLOR_GRAY);
    tft.drawText(64, 8, "Y=", COLOR_GRAY);
    tft.drawText(128, 8, "Z=", COLOR_GRAY);
  #endif
    tft.drawText(0, 16,  "Батарея=", COLOR_GRAY);
    tft.drawText(88, 16, "Зарядка=", COLOR_GRAY);
    oldOrientation = orientation;
  }

  switch (orientation) {
  case 2:
    return GravVector(vec.X, vec.Y, vec.Z);
  case 0:
    return GravVector(-vec.X, -vec.Y, vec.Z);
  case 1:
    return GravVector(-vec.Y, vec.X, vec.Z);
  case 3:
    return GravVector(vec.Y, -vec.X, vec.Z);
  default:
    return vec;
  }
}

/**
 * @brief setup
 */
void setup() {
  analogReference(INTERNAL);
  tft.begin();
  //delay(300);
  Serial.begin(9600);
//  Serial.begin(115200);
  Wire.begin();
#ifdef ADXL345_ENABLED
  accelBegin();
  setOrientation(accelReadVector());
#else
  setOrientation(GravVector());
#endif
}

/**
 * выводим батарейки.
 */
void printVolts() {
  // 1 сборка
//  double vBattery = analogRead(A7) * 0.00664;
//  double vCharger = analogRead(A6) * 0.00664;
  // 2 сборка
  double vBattery = analogRead(A7) * 0.00661;
  double vCharger = analogRead(A6) * 0.00654;
  // 3 сборка
//  double vBattery = analogRead(A7) * 0.00631;
//  double vCharger = analogRead(A6) * 0.00630;
#ifdef HAS_SERIAL
  Serial.print("vBattery = ");
  Serial.println(vBattery);
  Serial.print("vCharger = ");
  Serial.println(vCharger);
#endif
  drawDouble(52, 16, vBattery, COLOR_BLUE);
  drawDouble(140, 16, vCharger, COLOR_BLUEVIOLET);
}

void loop() {
  static long last = 0L;
  if (irControl.hasCode()) {
    long code = irControl.getCode();
    IrControlKey* controlKey = irControl.toControlKey(code);
    char key = 0;
    if (controlKey != NULL) {
      key = controlKey->key;
      if (key == 'p') {
        musicAlarm();
      } else {
        buzzerOut(controlKey->tone, 200, keySoundVolume);
      }
    }
    ltoa(code, comBuffer, 16);
    tft.drawText(135, 0, comBuffer, COLOR_CYAN);
//#ifdef HAS_SERIAL_DEBUG
    Serial.print("IR key = ");
    Serial.print(key);
    Serial.print("; code = ");
    Serial.println(code, HEX);
//#endif
  }
#ifdef ADXL345_ENABLED
  GravVector vec = setOrientation(accelReadVector());
#endif
  long time = millis();
  if (last != time / 1000) {
    //drawDouble(80, 0, time/1000.0, COLOR_BLUE);
    printRealTime();
    printRealDate();
    printVolts();
    last = time / 1000;
  }
#ifdef ADXL345_ENABLED
  accelUpdate(vec);
#endif
  delay(10);
}


