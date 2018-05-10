#include "SmartClock20.h"

// ������� ���������
TFT_LG_ILI9225 tft;

// ��������� ��� ��� �� �������
IrControl irControl(2);

char comBuffer[20];
uint8_t isRedraw = true;
uint16_t X0, X1, Y0, Y1;
uint16_t ClockX0, ClockX1, ClockY0, ClockY1;
uint16_t boxCenterX;
uint16_t boxCenterY;
GravVector gravVector;

// ������� �������
uint8_t currentCommand = 0;
ModeType mode = show;

ScreenTft* screens[NUMBER_OF_SCREENS] = {
  new ScreenDateTime(),
  new ScreenTimer()
};
ScreenTft * screen = screens[currentCommand];

/**
 * @brief printText ����� ������ �� �����
 * @param col ������� (x * 6)
 * @param row ������ (y * 8)
 * @param fontSize ������ ������
 * @param text ��������� �����
 * @param color
 */
void printText(uint8_t col, uint8_t row, uint8_t fontSize, const char * text, uint16_t color) {
  uint8_t oldFontSize = tft.getFontSize();
  uint16_t x = col * fontSize * FONT_SPACE + 1,
           y = row * fontSize * FONT_Y + 1;
  tft.setFontSize(fontSize);
  tft.drawText(x, y, text, color);
  tft.setFontSize(oldFontSize);
}

/**
 * @brief setCursor
 * @param col ������� (x * 6)
 * @param row ������ (y * 8)
 */
void setCursor(uint8_t col, uint8_t row, uint8_t fontSize) {
  static uint16_t x0 = -1, x1 = -1, y0 = -1, y1 = -1;
  //uint8_t fontSize = tft.getFontSize();
  tft.drawRectangle(x0, y0, x1, y1, tft.getBackgroundColor());
  x0 = col * fontSize * FONT_SPACE;
  x1 = x0 + fontSize * FONT_SPACE;
  y0 = row * fontSize * FONT_Y;
  y1 = y0 + fontSize * FONT_Y;
  tft.drawRectangle(x0, y0, x1, y1, COLOR_MAGENTA);
}

/**
 * @brief drawDouble
 * @param x
 * @param y
 * @param val
 * @param color
 */
void drawDouble(uint8_t col, uint8_t row, uint8_t fontSize, double val, uint16_t color) {
  //comBuffer[0] = val > 0 ? ' ' : '-';
  dtostrf(val, 5, 2, comBuffer);
  comBuffer[strlen(comBuffer)] = 0;
  printText(col, row, fontSize, comBuffer, color);
}

bool isHorisontalOrientation() {
  return tft.getOrientation() & ORIENTATION_HORISONTAL;
}

/**
 * @brief setOrientation
 * ���������� ���������� � ����������� �� ��������� ��������������� �������
 */
void setOrientation(GravVector vec) {
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

  switch (orientation) {
  case top:
  default:
    gravVector.set(vec.X, vec.Y, vec.Z);
    break;
  case bottom:
    gravVector.set(-vec.X, -vec.Y, vec.Z);
    break;
  case right:
    gravVector.set(-vec.Y, vec.X, vec.Z);
    break;
  case left:
    gravVector.set(vec.Y, -vec.X, vec.Z);
    break;
  }

  if (orientation != oldOrientation) {
    tft.clear();
    tft.setOrientation(orientation);
    screen->changeOrientation();
    screen->showOnce();
    oldOrientation = orientation;
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
 * ���������� ���������� �������� �� ������ � Serial
 */
void serIRkey(long code, char key) {
  ltoa(code, comBuffer, 16);
  if (key > 0) {
    uint16_t len = strlen(comBuffer);
    comBuffer[len++] = ' ';
    comBuffer[len++] = key;
    comBuffer[len++] = 0;
  }
  printText(20, 1, 1, comBuffer, COLOR_CYAN);
#ifdef HAS_SERIAL
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

    // ��������������
    if (mode == ModeType::edit) {
      screen->edit(key);
      return;
    }

    // ���������� �������
    switch (key) {
    // ������ �����
    case 'M':
      screen->edit(1);
//      screen->showOnce();
      break;
    // ������ �����
    case 'e':
      screen = changeCurrentCommand(true);
      break;
    default:
      screen->control(key);
      /*if (mode == ModeType::show) {
        screen->showOnce();
      }*/
      break;
    }
  }
  if (mode == ModeType::show) {
#ifdef ADXL345_ENABLED
    setOrientation(accelReadVector());
#endif
    static long lastTime = 0L;
    long time = millis();
    if (lastTime != time / 1000) {
      //drawDouble(12, 0, 1, time/1000.0, COLOR_BLUE);
      DateTime dateTime = RTClib().now();
      screen->showTime(&dateTime);
      lastTime = time / 1000;
      isRedraw = false;
    }
    screen->showEveryTime();
  }
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



