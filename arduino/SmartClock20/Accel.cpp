#include "SmartClock20.h"
#ifdef ADXL345_ENABLED
#include <Wire.h>
#include <ADXL345.h>

ADXL345 accel(ADXL345_ALT);

/**
 * @brief drawBobber
 * @param X
 * @param Y
 * @param Z
 * @param isReal
 */
void drawBobber(GravVector vec, bool isReal) {
  tft.drawCircle(
    BOX_CENTER_X + vec.X * 60,
    BOX_CENTER_Y + vec.Y * 60,
    7 - vec.Z * 3, isReal ? COLOR_CYAN : COLOR_BLACK
  );
/*
   tft.drawLine(
    BOX_CENTER_X,
    BOX_CENTER_Y,
    BOX_CENTER_X + vec.X * 60,
    BOX_CENTER_Y + vec.Y * 60,
    isReal ? COLOR_RED : COLOR_BLACK
  );
*/
}

/**
 * @brief drawBobber
 * @param X
 * @param Y
 * @param Z
 */
void drawBobber(GravVector vec) {
  static GravVector old;
  drawBobber(old, false);
  drawBobber(vec, true);
  old.X = vec.X;
  old.Y = vec.Y;
  old.Z = vec.Z;
}

/**
 * начальный старт
 */
void accelBegin() {

  byte deviceID = accel.readDeviceID();
#ifdef HAS_SERIAL
  if (deviceID != 0) {
    Serial.print("0x");
    Serial.print(deviceID, HEX);
    Serial.println("");
  } else {
    Serial.println("read device id: failed");
    return;
  }
#endif

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
#ifdef HAS_SERIAL
    Serial.println("write rate: failed");
#endif
    return;
  }

  // Data Range
  // - ADXL345_RANGE_2G: +-2 g
  // - ADXL345_RANGE_4G: +-4 g
  // - ADXL345_RANGE_8G: +-8 g
  // - ADXL345_RANGE_16G: +-16 g
  if (!accel.writeRange(ADXL345_RANGE_16G)) {
#ifdef HAS_SERIAL
    Serial.println("write range: failed");
#endif
    return;
  }

  if (!accel.start()) {
#ifdef HAS_SERIAL
    Serial.println("start: failed");
#endif
  }
}

/**
 * @brief accelReadVector
 * @return
 */
GravVector accelReadVector() {
  if (accel.update()) {
    return GravVector(-accel.getX(), accel.getY(), accel.getZ());
  } else {
#ifdef HAS_SERIAL
    Serial.println("update failed");
#endif
    return GravVector();
  }
}

/**
 * @brief accelUpdate
 */
void accelUpdate(GravVector vec) {
#ifdef HAS_SERIAL
    Serial.print(vec.X);
    Serial.print(",");
    Serial.print(vec.Y);
    Serial.print(",");
    Serial.print(vec.Z);
    Serial.println("");
#endif
    drawDouble(12, 16, -vec.X, COLOR_BLUE);
    drawDouble(70, 16, vec.Y, COLOR_GREEN);
    drawDouble(128, 16, vec.Z, COLOR_RED);
    drawBobber(vec);
}
#endif
