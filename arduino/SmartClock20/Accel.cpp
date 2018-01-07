#include <Wire.h>
#include <ADXL345.h>
#include "SmartClock20.h"

ADXL345 accel(ADXL345_ALT);

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

/**
 * @brief drawBobber
 * @param X
 * @param Y
 * @param Z
 * @param isReal
 */
void drawBobber(double X, double Y, double Z, bool isReal) {
  tft.fillCircle(
    BOX_CENTER_X + X * 60,
    BOX_CENTER_Y + Y * 60,
    7 - Z * 5, isReal ? COLOR_CYAN : COLOR_BLACK
  );
  tft.drawLine(
    BOX_CENTER_X,
    BOX_CENTER_Y,
    BOX_CENTER_X + X * 60,
    BOX_CENTER_Y + Y * 60,
    isReal ? COLOR_RED : COLOR_BLACK
  );
}

/**
 * @brief drawBobber
 * @param X
 * @param Y
 * @param Z
 */
void drawBobber(double X, double Y, double Z) {
  static double oldX = 0.0;
  static double oldY = 0.0;
  static double oldZ = 0.0;
  drawBobber(oldX, oldY, oldZ, false);
  drawBobber(X, Y, Z, true);
  oldX = X;
  oldY = Y;
  oldZ = Z;
}

/**
 * начальный старт
 */
void accelBegin() {

  Wire.begin();

  byte deviceID = accel.readDeviceID();
  if (deviceID != 0) {
    Serial.print("0x");
    Serial.print(deviceID, HEX);
    Serial.println("");
  } else {
    Serial.println("read device id: failed");
    return;
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
    return;
  }

  // Data Range
  // - ADXL345_RANGE_2G: +-2 g
  // - ADXL345_RANGE_4G: +-4 g
  // - ADXL345_RANGE_8G: +-8 g
  // - ADXL345_RANGE_16G: +-16 g
  if (!accel.writeRange(ADXL345_RANGE_16G)) {
    Serial.println("write range: failed");
    return;
  }

  if (!accel.start()) {
    Serial.println("start: failed");
  }
}

/**
 * @brief accelUpdate
 */
void accelUpdate() {
  if (accel.update()) {
    double X = accel.getX();
    double Y = accel.getY();
    double Z = accel.getZ();
    Serial.print(X);
    Serial.print(",");
    Serial.print(Y);
    Serial.print(",");
    Serial.print(Z);
    Serial.println("");
    drawFloat(12, 16, X, COLOR_BLUE);
    drawFloat(70, 16, Y, COLOR_GREEN);
    drawFloat(128, 16, Z, COLOR_RED);
    drawBobber(-X, Y, Z);
  } else {
    Serial.println("update failed");
    return;
  }
}
