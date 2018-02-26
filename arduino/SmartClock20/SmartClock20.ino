#include "SmartClock20.h"

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 tft(TFT_RST, TFT_RS, TFT_CS, TFT_LED);
// Use software SPI (slower)
//TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);

char comBuffer[20];

/**
 * @brief drawFloat
 * @param x
 * @param y
 * @param val
 * @param color
 */
void drawFloat(uint16_t x, uint16_t y, double val, uint16_t color) {
  char buf[10];
  //buf[0] = val > 0 ? ' ' : '-';
  dtostrf(val, 5, 2, buf);
  int len = strlen(buf);
  buf[len] = ' ';
  buf[len + 1] = 0;
  tft.drawText(x, y, buf, color);
}

void setup() {
  analogReference(INTERNAL);
  tft.begin();
  tft.setFont(Terminal6x8);
  tft.clear();
  tft.setOrientation(2);
  tft.drawRectangle(BOX_X0, BOX_Y0, BOX_X1, BOX_Y1, COLOR_WHITE);
#ifdef ADXL345_ENABLED
  tft.drawText(0, 16, "X=");
  tft.drawText(58, 16, "Y=");
  tft.drawText(116, 16, "Z=");
#endif
  tft.drawText(0, 28, "Battery=");
  tft.drawText(88, 28, "Charger=");

  //delay(300);
  Serial.begin(9600);
  Wire.begin();
#ifdef ADXL345_ENABLED
  accelBegin();
#endif
}

/**
 * выводим реальное время.
 */
void printVolts() {
  // 1 сборка
  double vBattery = analogRead(A7) * 0.00664;
  double vCharger = analogRead(A6) * 0.00664;
  // 2 сборка
//  double vBattery = analogRead(A7) * 0.00661;
//  double vCharger = analogRead(A6) * 0.00654;
  // 3 сборка
//  double vBattery = analogRead(A7) * 0.00631;
//  double vCharger = analogRead(A6) * 0.00630;
  Serial.print("vBattery = ");
  Serial.println(vBattery);
  Serial.print("vCharger = ");
  Serial.println(vCharger);
  drawFloat(52, 28, vBattery, COLOR_BLUE);
  drawFloat(140, 28, vCharger, COLOR_BLUEVIOLET);
}

void loop() {
  //tft.fillRectangle(BOX_X0, 0, BOX_X1, BOX_Y0 - 1, COLOR_GRAY);
  //tft.drawText(0, 0, "hello!");
  printRealTime();
  printRealDate();
  printVolts();
#ifdef ADXL345_ENABLED
  accelUpdate();
#endif
  delay(1000);
}


