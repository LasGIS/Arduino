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
  uint16_t x = vec.X, y = vec.Y, z = vec.Z,
      color = isReal ? COLOR_CYAN : COLOR_BLACK;
  tft.drawRectangle(x - z + 1, y - z + 1, x + z, y + z, color);
//  tft.drawCircle(x, y, z, color);
}
/**
 * @brief calcMoving
 * @param old старые координаты м€чика
 * @param grv
 * @return новые координаты м€чика
 */
GravVector calcMoving(GravVector old, GravVector grv) {
  static double vx = 0.0, vy = 0.0;
  static long timePoint = millis();
  double dX0 = X0, dX1 = X1, dY0 = ClockY1, dY1 = Y1;
  long deltaTime = millis() - timePoint;
  timePoint += deltaTime;
  double dTime = deltaTime / 500.0;
  old.X = old.X + vx * dTime + grv.X * dTime * dTime / 2;
  old.Y = old.Y + vy * dTime + grv.Y * dTime * dTime / 2;
  vx = vx + grv.X * dTime;
  vy = vy + grv.Y * dTime;
  old.Z = 10 + grv.Z * 3;
  if (old.X > (dX1 - old.Z)) {
    old.X = (dX1 - old.Z) * 2 - old.X;
//    old.X = dX1 - old.Z;
    vx = grv.X * dTime - vx;
  }
  if (old.X < (dX0 + old.Z)) {
    old.X = (old.Z + dX0) * 2 - old.X;
//    old.X = old.Z + dX0;
    vx = grv.X * dTime - vx;
  }
  if (old.Y > (dY1 - old.Z)) {
    old.Y = (dY1 - old.Z) * 2 - old.Y;
//    old.Y = dY1 - old.Z;
    vy = grv.Y * dTime - vy;
  }
  if (old.Y < (dY0 + old.Z)) {
    old.Y = (dY0 + old.Z) * 2 - old.Y;
//    old.Y = dY0 + old.Z;
    vy = grv.Y * dTime - vy;
  }
#ifdef HAS_SERIAL
  Serial.print("old.X = ");
  Serial.print(old.X);
  Serial.print(", old.Y = ");
  Serial.print(old.Y);
  Serial.println(";");
#endif
  return old;
//  return GravVector(boxCenterX + grv.X * 60, boxCenterY + grv.Y * 60, 7 - grv.Z * 3);
}

/**
 * @brief –исуем прыгающий м€чик
 * @param vec вектор гравитации
 */
void drawBobber(GravVector grv) {
  static GravVector old(boxCenterX, boxCenterY, 7);
  GravVector crd = calcMoving(old, grv);
  drawBobber(old, false);
  drawBobber(crd, true);
  old = crd;
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
#else
  if (deviceID == 0) {
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
void accelUpdate(GravVector grav) {
#ifdef HAS_SERIAL
    Serial.print(grav.X);
    Serial.print(",");
    Serial.print(grav.Y);
    Serial.print(",");
    Serial.print(grav.Z);
    Serial.println(";");
#endif
    drawDouble(2,  1, grav.X, COLOR_BLUE);
    drawDouble(10, 1, grav.Y, COLOR_GREEN);
    drawDouble(18, 1, grav.Z, COLOR_RED);
    drawBobber(grav);
}
#endif
