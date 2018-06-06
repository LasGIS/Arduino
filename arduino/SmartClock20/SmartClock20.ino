#include "SmartClock20.h"

// драйвер экранчика
TFT_LG_ILI9225 tft;

// указываем пин для ИК датчика
IrControl irControl(2);

char comBuffer[20];
uint8_t isSerial = false;
uint8_t isRedraw = true;
uint16_t X0, X1, Y0, Y1;
uint16_t ClockX0, ClockX1, ClockY0, ClockY1;
uint16_t boxCenterX;
uint16_t boxCenterY;
GravVector gravVector;

// текущая команда
uint8_t currentCommand = 0;
ModeType mode = show;

ScreenTft* screens[NUMBER_OF_SCREENS] = {
  new ScreenDateTime(),
  new ScreenTimer(),
  new ScreenDump()
};
ScreenTft * screen = screens[currentCommand];

/**
 * @brief printText вывод текста на экран
 * @param col колонка (x * 6)
 * @param row строка (y * 8)
 * @param fontSize размер щрифта
 * @param text выводимый текст
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
 * @param col колонка (x * 6)
 * @param row строка (y * 8)
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
  dtostrf(val, 5, 2, comBuffer);
//  comBuffer[strlen(comBuffer)] = 0;
  printText(col, row, fontSize, comBuffer, color);
}

void drawHex(uint8_t col, uint8_t row, uint8_t fontSize, uint16_t val, uint8_t size, uint16_t color) {
  for (int8_t i = size - 1; i >= 0; i--) {
    uint8_t b = val & 0xf;
    comBuffer[i] = b < 10 ? '0' + b : 'A' + b - 10;
    val >>= 4;
  }
  comBuffer[size] = 0;
  printText(col, row, fontSize, comBuffer, color);
}

bool isHorisontalOrientation() {
  return tft.getOrientation() & ORIENTATION_HORISONTAL;
}

/**
 * @brief setOrientation
 * Поправляем ориентацию в зависимости от показаний гравитационного датчика
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
  Wire.begin();
  tft.begin();

  Serial.begin(9600);
//  Serial.begin(115200);
#ifdef ADXL345_ENABLED
  accelBegin();
  setOrientation(accelReadVector());
#else
  setOrientation(GravVector());
#endif
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

    // редактирование
    if (mode == ModeType::edit) {
      screen->edit(key);
      if (mode == ModeType::show) {
        screen->showOnce();
      } else {
        screen->showCurrentField();
      }
      return;
    }

    // управление экраном
    switch (key) {
    // меняем режим
    case 'M':
      screen->edit(1);
      screen->showOnce();
      screen->showAllFields();
      screen->showCurrentField();
      break;
    // меняем экран
    case 'e':
      screen = changeCurrentCommand(true);
      break;
    default:
      screen->control(key);
      break;
    }
  }
  if (mode == ModeType::show && !isSerial) {
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

/**
 * @brief serialEvent
#define MAX_LOAD_SIZE 100
void serialEvent() {
//  static uint8_t outRow = 7;
  char buf[MAX_LOAD_SIZE];
  isSerial = true;
  while (Serial.available() > 0) {
    int cnt = Serial.readBytes(buf, MAX_LOAD_SIZE);
    if (cnt >= 0 && cnt < MAX_LOAD_SIZE) {
      buf[cnt] = 0;
    }
  }
//#ifdef HAS_SERIAL
  Serial.print('[');
  Serial.print(buf);
  Serial.println(']');
//#endif
//  printText(1, outRow, 1, buf, COLOR_GOLD);
//  outRow = outRow < 17 ? outRow + 1 : 7;
  isSerial = false;
}
*/

/**
 * @brief serialEvent
 */
void serialEvent() {
  if (Serial.available() && Serial.read() == 0x45) {
    byte bt; if (Serial.readBytes(&bt, 1) == 1 && bt == 0x42) {
      SerialBlock * block = I2CEEPROM.serialReadBlock();
      if (block != NULL) {
        // обязательный ответ
        Serial.print(":adr=");
        Serial.println(block->address);
        I2CEEPROM.write_buffer(block->device, block->address, block->body, block->size);

        Serial.print("block size = ");
        Serial.print(block->size);
        Serial.print("; device = ");
        Serial.print(block->device, HEX);
        Serial.print("; address = ");
        Serial.print(block->address, HEX);
        Serial.print("; cs = ");
        Serial.println(block->cs);
        Serial.print("; body = ");
        Serial.print((int) block->body, HEX);
        Serial.print("; \"");
        for (int i = 0; i < block->size; i++) {
          SerialPrintHex(block->body[i]);
        }
        Serial.println("\"");

        delete block;
      } else {
        Serial.println("block === null");
      }
  //  } else if (mark != -1) {
  //    Serial.print("mark = ");
  //    Serial.println(mark, HEX);
  //    delay(1000);
    } else {
      Serial.print("byte = ");
      Serial.println(bt, HEX);
    }
  }

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
