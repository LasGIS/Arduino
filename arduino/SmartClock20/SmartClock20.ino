// Include application, user and local libraries
#include <Wire.h>
#include "SPI.h"
#include "TFT_22_ILI9225.h"
#include <iarduino_RTC.h>
#include <ADXL345.h>

#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 3   // 0 if wired to +5V directly

ADXL345 accel(ADXL345_ALT);

//  iarduino_RTC time(RTC_DS1302, 1, 2, 3); // подключаем RTC модуль на базе чипа DS1302, указывая выводы Arduino подключённые к выводам модуля RST, CLK, DAT
//  iarduino_RTC time(RTC_DS1307);          // подключаем RTC модуль на базе чипа DS1307, используется аппаратная шина I2C
    iarduino_RTC time(RTC_DS3231);          // подключаем RTC модуль на базе чипа DS3231, используется аппаратная шина I2C

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED);
// Use software SPI (slower)
//TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);
#define BOX_X0 0
#define BOX_X1 175
#define BOX_Y0 44
#define BOX_Y1 219
char comBuffer[25];

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
  time.begin();
  Wire.begin();

  byte deviceID = accel.readDeviceID();
  if (deviceID != 0) {
    Serial.print("0x");
    Serial.print(deviceID, HEX);
    Serial.println("");
  } else {
    Serial.println("read device id: failed");
    while(1) {
      delay(100);
    }
  }

  // Data Rate
  // - ADXL345_RATE_3200HZ: 3200 Hz
  // - ADXL345_RATE_1600HZ: 1600 Hz
  // - ADXL345_RATE_800HZ:  800 Hz
  // - ADXL345_RATE_400HZ:  400 Hz
  // - ADXL345_RATE_200HZ:  200 Hz
  // - ADXL345_RATE_100HZ:  100 Hz
  // - ADXL345_RATE_50HZ:   50 Hz
  // - ADXL345_RATE_25HZ:   25 Hz
  // - ...
  if (!accel.writeRate(ADXL345_RATE_200HZ)) {
    Serial.println("write rate: failed");
    while(1) {
      delay(100);
    }
  }

  // Data Range
  // - ADXL345_RANGE_2G: +-2 g
  // - ADXL345_RANGE_4G: +-4 g
  // - ADXL345_RANGE_8G: +-8 g
  // - ADXL345_RANGE_16G: +-16 g
  if (!accel.writeRange(ADXL345_RANGE_16G)) {
    Serial.println("write range: failed");
    while(1) {
      delay(100);
    }
  }

  if (!accel.start()) {
    Serial.println("start: failed");
    while(1) {
      delay(100);
    }
  }
}

void loop() {
  //tft.fillRectangle(BOX_X0, 0, BOX_X1, BOX_Y0 - 1, COLOR_GRAY);
  //tft.drawText(0, 0, "hello!");
  char* timeString = time.gettime("d/m/Y H:i:s, D");
  Serial.println(timeString); // выводим время
  tft.drawText(0, 4, timeString);
  if (accel.update()) {
    double accelX = accel.getX();
    double accelY = accel.getY();
    double accelZ = accel.getZ();
    Serial.print(accelX);
    Serial.print(",");
    Serial.print(accelY);
    Serial.print(",");
    Serial.print(accelZ);
    Serial.println("");
    drawFloat(12, 16, accelX, COLOR_BLUE);
    drawFloat(70, 16, accelY, COLOR_GREEN);
    drawFloat(128, 16, accelZ, COLOR_RED);
  } else {
    Serial.println("update failed");
    while(1) {
      delay(100);
    }
  }
  delay(200);
}

void drawFloat(uint16_t x, uint16_t y, double val, uint16_t color) {
  char buf[10];
  //buf[0] = val > 0 ? ' ' : '-';
  dtostrf(val, 5, 2, buf);
  int len = strlen(buf);
  buf[len] = ' ';
  buf[len + 1] = 0;
  tft.drawText(x, y, buf, color);
}

