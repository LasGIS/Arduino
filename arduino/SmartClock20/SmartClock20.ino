#include "SmartClock20.h"

//ADXL345 accel(ADXL345_ALT);

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 tft(TFT_RST, TFT_RS, TFT_CS, TFT_LED);
// Use software SPI (slower)
//TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);

char comBuffer[20];

void setup() {
  tft.begin();
  tft.setFont(Terminal6x8);
  tft.clear();
  tft.setOrientation(2);
  tft.drawRectangle(BOX_X0, BOX_Y0, BOX_X1, BOX_Y1, COLOR_WHITE);
  tft.drawText(0, 16, "X=");
  tft.drawText(58, 16, "Y=");
  tft.drawText(116, 16, "Z=");

  //delay(300);
  Serial.begin(9600);
  accelBegin();
}

void loop() {
  //tft.fillRectangle(BOX_X0, 0, BOX_X1, BOX_Y0 - 1, COLOR_GRAY);
  //tft.drawText(0, 0, "hello!");
  printRealTime();
  printRealDate();
  accelUpdate();
  delay(200);
}


