#include "SmartClock20.h"

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 tft(TFT_RST, TFT_RS, TFT_CS, TFT_LED);
// Use software SPI (slower)
//TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);

char comBuffer[20];
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
  char buf[10];
  //buf[0] = val > 0 ? ' ' : '-';
  dtostrf(val, 5, 2, buf);
  int len = strlen(buf);
  buf[len] = ' ';
  buf[len + 1] = 0;
  tft.drawText(x, y, buf, color);
}

/**
 * @brief setOrientation
 * Поправляем ориентацию в зависимости от показаний гравитационного датчика
 */
GravVector setOrientation(GravVector vec) {
  static uint8_t oldOrientation = 2;
  uint8_t orientation = 0;
  uint16_t X0, X1, Y0, Y1;
  if (vec.Y > GRAVI_FACTOR) {
    orientation = 2;
  } else if (vec.Y < -GRAVI_FACTOR) {
    orientation = 0;
  } else if (vec.X > GRAVI_FACTOR) {
    orientation = 1;
  } else if (vec.X < -GRAVI_FACTOR) {
    orientation = 3;
  } else {
    orientation = oldOrientation;
  }
  Serial.println(orientation);
  switch (orientation) {
  default:
  case 2:
  case 0:
    X0 = BOXV_X0;
    X1 = BOXV_X1;
    Y0 = BOXV_Y0;
    Y1 = BOXV_Y1;
    boxCenterX = BOXV_CENTER_X;
    boxCenterY = BOXV_CENTER_Y;
    break;
  case 1:
  case 3:
    X0 = BOXH_X0;
    X1 = BOXH_X1;
    Y0 = BOXH_Y0;
    Y1 = BOXH_Y1;
    boxCenterX = BOXH_CENTER_X;
    boxCenterY = BOXH_CENTER_Y;
    break;
  }
  if (orientation != oldOrientation) {
    tft.clear();
    tft.setOrientation(orientation);
    tft.drawRectangle(X0, Y0, X1, Y1, COLOR_WHITE);
  #ifdef ADXL345_ENABLED
    tft.drawText(0, 16, "X=");
    tft.drawText(58, 16, "Y=");
    tft.drawText(116, 16, "Z=");
  #endif
    tft.drawText(0, 28, "Battery=");
    tft.drawText(88, 28, "Charger=");
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
  tft.setFont(Terminal6x8);
  //delay(300);
  Serial.begin(9600);
  Wire.begin();
#ifdef ADXL345_ENABLED
  accelBegin();
  setOrientation(accelReadVector());
#else
  setOrientation(GravVector());
#endif
}

/**
 * выводим реальное время.
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
  drawDouble(52, 28, vBattery, COLOR_BLUE);
  drawDouble(140, 28, vCharger, COLOR_BLUEVIOLET);
}

void loop() {
  static long last = 0L;
  long time = millis();
#ifdef ADXL345_ENABLED
  GravVector vec = setOrientation(accelReadVector());
#endif
  if (last != time / 1000) {
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


