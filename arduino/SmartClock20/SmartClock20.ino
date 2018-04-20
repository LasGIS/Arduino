#include "SmartClock20.h"

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
TFT_LG_ILI9225 tft(TFT_RST, TFT_RS, TFT_CS, TFT_LED);

// указываем пин для ИК датчика
IrControl irControl(2);

char comBuffer[20];
uint8_t isChangeOrientation = true;
uint16_t X0, X1, Y0, Y1;
uint16_t ClockX0, ClockX1, ClockY0, ClockY1;
uint16_t clockX;
uint16_t clockY;
uint16_t boxCenterX;
uint16_t boxCenterY;

// текущая команда
uint8_t currentCommand = 0;
ModeType mode = show;

ScreenTft* screens[NUMBER_OF_SCREENS] = {
  new ScreenDateTime(),
  new ScreenTimer()
};
ScreenTft * screen = screens[currentCommand];

/**
 * @brief printText вывод текста на экран
 * @param col колонка (x * 6)
 * @param row строка (y * 8)
 * @param text выводимый текст
 * @param color
 */
void printText(uint16_t col, uint16_t row, const char * text, uint16_t color) {
  uint8_t fontSize = tft.getFontSize();
  uint16_t x = col * fontSize * FONT_SPACE;
  uint16_t y = row * fontSize * FONT_Y;
  tft.drawText(x, y, text, color);
}

/**
 * @brief drawDouble
 * @param x
 * @param y
 * @param val
 * @param color
 */
void drawDouble(uint16_t col, uint16_t row, double val, uint16_t color) {
  //comBuffer[0] = val > 0 ? ' ' : '-';
  dtostrf(val, 5, 2, comBuffer);
  int len = strlen(comBuffer);
  comBuffer[len] = ' ';
  comBuffer[len + 1] = 0;
  printText(col, row, comBuffer, color);
}

/**
 * @brief setOrientation
 * Поправляем ориентацию в зависимости от показаний гравитационного датчика
 */
GravVector setOrientation(GravVector vec) {
  static OrientationType oldOrientation = undefine;
  static OrientationType orientation = top;
  if (vec.Y > GRAVI_FACTOR) {
    orientation = top;
  } else if (vec.Y < -GRAVI_FACTOR) {
    orientation = bottom;
  } else if (vec.X > GRAVI_FACTOR) {
    orientation = right;
  } else if (vec.X < -GRAVI_FACTOR) {
    orientation = left;
  }

  if (orientation != oldOrientation) {
    isChangeOrientation = true;
    tft.clear();
    tft.setOrientation(orientation);
    screen->changeOrientation(orientation);
    screen->showOnce();
    oldOrientation = orientation;
  }

  switch (orientation) {
  case top:
    return GravVector(vec.X, vec.Y, vec.Z);
  case bottom:
    return GravVector(-vec.X, -vec.Y, vec.Z);
  case right:
    return GravVector(-vec.Y, vec.X, vec.Z);
  case left:
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
  double vBattery = analogRead(A7) * 0.00664;
  double vCharger = analogRead(A6) * 0.00664;
  // 2 сборка
//  double vBattery = analogRead(A7) * 0.00661;
//  double vCharger = analogRead(A6) * 0.00654;
  // 3 сборка
//  double vBattery = analogRead(A7) * 0.00631;
//  double vCharger = analogRead(A6) * 0.00630;
#ifdef HAS_SERIAL
  Serial.print("vBattery = ");
  Serial.println(vBattery);
  Serial.print("vCharger = ");
  Serial.println(vCharger);
#endif
  drawDouble(4, 1, vBattery, COLOR_BLUE);
  drawDouble(15, 1, vCharger, COLOR_BLUEVIOLET);
}

/**
 * Показываем полученное значение ИК пульта в Serial
 */
void serIRkey(long code, char key) {
  ltoa(code, comBuffer, 16);
  if (key > 0) {
    uint16_t len = strlen(comBuffer);
    comBuffer[len++] = ' ';
    comBuffer[len++] = key;
    comBuffer[len++] = 0;
  }
  printText(22, 0, comBuffer, COLOR_CYAN);
#ifdef HAS_SERIAL_DEBUG
  Serial.print("IR key = ");
  Serial.print(key);
  Serial.print("; code = ");
  Serial.println(code, HEX);
#endif
}

/**
 * @brief loop
 */
void loop() {

  if (irControl.hasCode()) {
    long code = irControl.getCode();
    IrControlKey* controlKey = irControl.toControlKey(code);
    char key = 0;
    if (controlKey != NULL) {
      key = controlKey->key;
      buzzerOut(controlKey->tone, 200, keySoundVolume);
    }
    serIRkey(code, key);

    // редактирование
    if (mode == edit) {
      screen->edit(key);
      return;
    }

    // управление экраном
    switch (key) {
    case 'M':
      screen = changeCurrentCommand(true);
      break;
    case 'e':
      screen = changeCurrentCommand(false);
      break;
    default:
      screen->control(key);
      /*if (mode == show) {
        screen->showOnce();
      }*/
      break;
    }
  }
#ifdef ADXL345_ENABLED
  GravVector vec = setOrientation(accelReadVector());
#endif
  static long lastTime = 0L;
  long time = millis();
  if (lastTime != time / 1000) {
    //drawDouble(12, 0, time/1000.0, COLOR_BLUE);
    printRealTime();
    printRealDate();
    printVolts();
    lastTime = time / 1000;
    isChangeOrientation = false;
  }

  screen->showEveryTime();
#ifdef ADXL345_ENABLED
  accelUpdate(vec);
#endif
  delay(10);
}

ScreenTft * changeCurrentCommand(bool isIncrement) {
  if (isIncrement) {
    currentCommand = (currentCommand < NUMBER_OF_SCREENS - 1) ? currentCommand + 1 : 0;
  } else {
    currentCommand = currentCommand > 0 ? currentCommand - 1 : NUMBER_OF_SCREENS - 1;
  }
  ScreenTft * screen = screens[currentCommand];
  screen->showOnce();
  return screen;
}



