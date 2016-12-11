/*****************************************
 * Ловим температуру и влажность,        *
 * выводим её на TFT Экран и пишем в SD. *
 *****************************************/

//#include <Arduino.h>
#include <avr/pgmspace.h>
#include <SPI.h>
#include <SD.h>
#include <DHT.h>
#include <IrControl.h>
#include "WeatherSaveShow.h"

char buffer[20];

File myFile;

/**
 ** SD карта подключается так (подключение пинов по умолчанию):
 ** MOSI - пин 11
 ** MISO - пин 12
 ** CLK - пин 13
 ** CS - пин A3
 */
// pin definition for SD
#define sd_cs  A3

// pin definition for screen TFT
#define tft_cs   10
#define tft_dc   9
#define tft_rst  8

// create an instance of the screen TFT library
TFT screen = TFT(tft_cs, tft_dc, tft_rst);

/** настраиваем измеритель влажности. */
DHT dht1(7, DHT22);
DHT dht2(6, DHT22);

// указываем пин для ИК датчика
IrControl irControl(2);

/** настраиваем real time clock. */
const int kCePin   = 3;  // Chip Enable
const int kIoPin   = 4;  // Input/Output
const int kSclkPin = 5;  // Serial Clock
DS1302 rtc(kCePin, kIoPin, kSclkPin);

int curX = screenLeft - 1;
int curNextX = curX + 1;
int curNextY = screenTop;
const char mainScreenText[] = "Т1=    C В1=    %         \nТ2=    C В2=    % V=     V";
//const char timeSetScreenText[] = "********\n********";

void setup() {
#ifdef HAS_SERIAL
  // begin serial communication
  Serial.begin(9600);
#endif
  // Put this line at the beginning of every sketch that uses the GLCD:
  screen.begin(3);

  // clear the screen with a black background
  screen.background(backColor);

  screen.stroke(textColor);
  screen.setTextSize(1);
  screen.text(mainScreenText, 0, 0);
  drawGrid();

  initSD();

  dht1.begin();
  dht2.begin();

  irControl.start();
}

void loop() {
  static int count = 0;
  static bool isEditTime = false;
  if (--count < 0) {
    if (!isEditTime) {
      temperatureHumidity();
    }
    count = 60;
  }
  if (irControl.hasCode()) {
    long code = irControl.getCode();
    IrControlKey* irControlKey = irControl.toControlKey(code);
    char key = irControlKey->key;
    switch (key) {
    case 'm':
    case 'b':
      screen.stroke(textColor);
      screen.fillRect(0, 0, 160, 16, backColor);
      showScreen(isEditTime = !isEditTime);
      break;
    default:
      if (isEditTime) {
        bool isNewEditTime = editTime(key);
        if (!isNewEditTime) {
          isEditTime = isNewEditTime;
          showScreen(isEditTime);
        }
      }
      break;
    }
  }
  delay(100);
}

/**
 * Первоначальный показ экрана
 */
void showScreen(bool isEditTime) {
  if (isEditTime) {
    editTime(1);
  } else {
    screen.text(mainScreenText, 0, 0);
  }
}

/**
 * зачищаем место для надписи
 */
void fillPlace(int x, int y, int len, color col) {
  screen.stroke(col);
  screen.fillRect(x * 6, y * 8, (len * 6) - 1, 7, 0);
  screen.setCursor(x * 6, y * 8);
}

/**
 * показываем решётку
 */
void drawGrid() {
  screen.drawFastVLine(screenLeft,  screenTop,    100, markHourColor);
  screen.drawFastVLine(screenRigth, screenTop,    100, voltColor);
  screen.drawFastHLine(screenLeft,  screenTop,    144, voltColor);
  screen.drawFastHLine(screenLeft,  screenBottom, 144, voltColor);
  screen.stroke(colorT1);
  for (float temp = TEMPERATURE_START; temp <= TEMPERATURE_START + 100 / TEMPERATURE_MULTIPLIER; temp += 10) {
    int ty = (int) (screenBottom - (temp - TEMPERATURE_START) * TEMPERATURE_MULTIPLIER);
    screen.setCursor((temp >= 0.0 ? screenRigth + 3 : screenRigth - 1), ty - 3);
    screen.print(temp, 0);
  }
  screen.stroke(colorTime);
  for (int time = 0; time <= 24; time += 3) {
    int tx = (int) (screenLeft + time * 6);
    screen.setCursor(time < 10 ? (time == 0 ? tx : tx - 2) : tx - 6, screenBottom + 2);
    screen.print(time);
  }
}

/**
 * Инициализация SD
 */
void initSD() {
#ifdef HAS_SERIAL
  Serial.print("Initializing SD card...");
#endif
  if (!SD.begin(sd_cs)) {
    screen.stroke(0, 0, 255);
#ifdef HAS_SERIAL
    Serial.println("initialization SD card failed!");
#endif
    screen.text("init SD card failed!", screenLeft + 1, screenTop + 1);
    return;
  }
#ifdef HAS_SERIAL
  Serial.println("initialization done.");
#endif
  screen.stroke(255, 120, 120);
  screen.text("init SD card done.", screenLeft + 1, screenTop + 1);
}

/**
 * Получаем значение напряжения питания
 */
float findVolt() {
  float factor = 2.5 / analogRead(A7);
  return analogRead(A6) * factor;;
}

/**
 * сохраняем на SD
 */
void saveData(Time time, float volt, float temp1, float hum1, float temp2, float hum2) {
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print("time=");
    dataFile.print(printTime(&time, DataTime));
    dataFile.print(",T1=");
    dataFile.print(temp1, 1);
    dataFile.print(",H1=");
    dataFile.print(hum1, 1);
    dataFile.print(",T2=");
    dataFile.print(temp2, 1);
    dataFile.print(",H2=");
    dataFile.print(hum2, 1);
    dataFile.print(",V=");
    dataFile.println(volt, 2);
    dataFile.close();
    //screen.drawPixel(curNextX, curNextY, foneColor);
  } else {
#ifdef HAS_SERIAL
    Serial.println("error on datalog.txt");
#endif
/*
    fillPlace(0, 2, 27, screen.stroke(0, 0, 255));
    screen.print("error on datalog.txt", 0, 16);
*/
    screen.drawPixel(curNextX, curNextY, errorColor);
    curNextY++; if (curNextY > screenBottom) curNextY = screenTop;
  }
}

/**
 * показываем на экране
 */
void showData(long time, float volt, float temp1, float hum1, float temp2, float hum2, bool isVline) {
  if (isVline) {
    uint16_t color;
    // 3 часа
    if (time % 10800000 < TIME_MULTIPLIER) {
      color = markHourColor;
    }
    // 1 час 900000; // 15 мин
    else if (time % 3600000 < TIME_MULTIPLIER) {
      color = markMinColor;
    }
    else {
      color = foneColor;
    }
    screen.drawFastVLine(curX, screenTop, 100, color);
    showTempMarks();
  }
  if (!isnan(volt)) showVolt(volt, voltColor);
  if (!isnan(hum1)) showHum(hum1, colorB1);
  if (!isnan(hum2)) showHum(hum2, colorB2);
  if (!isnan(temp1)) showTemp(temp1, colorT1);
  if (!isnan(temp2)) showTemp(temp2, colorT2);
}

void showTempMarks() {
  for (float temp = TEMPERATURE_START; temp <= TEMPERATURE_START + 100 / TEMPERATURE_MULTIPLIER; temp += 5.0) {
    showTemp(temp, ((int) temp % 10 == 0) ? markTempColor : markMinColor);
  }
}

void showTemp(float temp, color col) {
  int ty = (int) (screenBottom - (temp - TEMPERATURE_START) * TEMPERATURE_MULTIPLIER);
  screen.drawPixel(curX, ty, col);
}

void showHum(float hum, color col) {
  int hy = (int) (screenBottom - hum);
  screen.drawPixel(curX, hy, col);
}

void showVolt(float volt, color col) {
  int vy = (int) (screenBottom - (volt - 2.0) * 40);
  screen.drawPixel(curX, vy, col);
}

/**
 * Показываем температуру и влажность
 */
void temperatureHumidity() {
  static int cntX = 0;
  float volt = findVolt();
  Time time = rtc.time();
  long lTime = (((time.hr * 60l + time.min) * 60) + time.sec) * 1000;
  double temp1 = dht1.readTemperature();
  double hum1  = dht1.readHumidity();
  double temp2 = dht2.readTemperature();
  double hum2  = dht2.readHumidity();

  saveData(time, volt, temp1, hum1, temp2, hum2);

  if (curX != lTime / TIME_MULTIPLIER) {
    curX = lTime / TIME_MULTIPLIER;
    if (curX >= screenRigth) curX = screenLeft;
    curNextX = curX + 1;
    if (curNextX >= screenRigth) curNextX = screenLeft;
    curNextY = screenTop;
    screen.drawFastVLine(curNextX, screenTop, 101, markColor);
    cntX = 0;
  }
  showData(lTime, volt, temp1, hum1, temp2, hum2, cntX == 0);
  cntX++;

  fillPlace(18, 0, 8, colorTime);
  screen.print(printTime(&time, TimeMode));
  fillPlace(3, 0, 4, colorT1);
  screen.print(temp1, 1);
  fillPlace(12, 0, 4, colorB1);
  screen.print(hum1, 1);
  fillPlace(3, 1, 4, colorT2);
  screen.print(temp2, 1);
  fillPlace(12, 1, 4, colorB2);
  screen.print(hum2, 1);
  fillPlace(20, 1, 5, voltColor);
  screen.print(volt, 2);

#ifdef HAS_SERIAL
  Serial.print("Volt: ");
  Serial.print(volt);
  Serial.print("V;\tTemp1: ");
  Serial.print(temp1);
  Serial.print("*C;\tHum1: ");
  Serial.print(hum1);
  Serial.print("%;\tTemp2: ");
  Serial.print(temp2);
  Serial.print("*C;\tHum2: ");
  Serial.print(hum2);
  Serial.println("%;");
#endif
}
