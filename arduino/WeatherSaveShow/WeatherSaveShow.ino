/**
 * Читаем MicroSD на TFT Экран
 * SD карта подключается так:
 ** MOSI - пин 11
 ** MISO - пин 12
 ** CLK - пин 13
 ** CS - пин 4
 */
//#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <TFT_lg.h>
#include <DHT.h>

File myFile;

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// create an instance of the library
TFT screen = TFT(cs, dc, rst);

/** настраиваем измеритель влажности. */
DHT dht1(7, DHT22);
DHT dht2(6, DHT22);

//#define HAS_SERIAL
#define TEMPERATURE_START -10.0
#define TEMPERATURE_MULTIPLIER 2.0
/* весь экран - 8 часов */
//#define TIME_MULTIPLIER 180000
/* весь экран - 24 часа */
#define TIME_MULTIPLIER 600000

#define colorT1       0b0000000111011111
#define colorT2       0b0111100111011111
#define colorB1       0b0000011111100000
#define colorB2       0b0111111111100000
#define colorTime     0b0111101111101111
#define voltColor     0b1111100100000100
#define foneColor     0b0001100011100011
#define errorColor    0b0000000000011111
#define markColor     0b1111111111111111
#define markTempColor 0b1001010010010010
#define markMinColor  0b0110001100001100
#define markHourColor 0b1001010010010010

#define screenTop     16
#define screenLeft    12
#define screenRigth   156
#define screenBottom  116

int curX = screenLeft;

void setup() {
#ifdef HAS_SERIAL
  // begin serial communication
  Serial.begin(9600);
#endif
  // Put this line at the beginning of every sketch that uses the GLCD:
  screen.begin(3);

  // clear the screen with a black background
  screen.background(0, 0, 0);

  screen.stroke(255, 255, 255);
  screen.setTextSize(1);
  //screen.text("Напряжение=10.00V\nТемпер.=    C Влажн.=    %", 0, 0);
  screen.text("Т1=    C В1=    %        м\nТ2=    C В2=    % V=     V", 0, 0);
  drawGrid();

  initSD();

  dht1.begin();
  dht2.begin();
}

void loop() {
  // Read the value of the sensor
  float factor = 2.5 / analogRead(A7);
  float sensor = analogRead(A6) * factor;

  temperatureHumidity(sensor);

  fillPlace(20, 1, 5, voltColor);
  screen.print(sensor, 2);

  delay(2000);
}

/**
 * зачищаем место для надписи
 */
void fillPlace(int x, int y, int len, color col) {
  screen.stroke(col);
  screen.fillRect(x * 6, y * 8, (len * 6) - 1, 7, 0);
  screen.text("", x * 6, y * 8);
}

/**
 * показываем решётку
 */
void drawGrid() {
  screen.drawFastVLine(screenLeft - 1, screenTop,    100, markTempColor);
  screen.drawFastVLine(screenRigth,    screenTop,    100, markTempColor);
  screen.drawFastHLine(screenLeft - 1, screenTop,    146, markTempColor);
  screen.drawFastHLine(screenLeft - 1, screenBottom, 146, markTempColor);

}

/**
 * Инициализация SD
 */
void initSD() {
#ifdef HAS_SERIAL
  Serial.print("Initializing SD card...");
#endif
  if (!SD.begin(4)) {
    screen.stroke(0, 0, 255);
#ifdef HAS_SERIAL
    Serial.println("initialization SD card failed!");
#endif
    screen.text("init SD card failed!", 0, 16);
    return;
  }
#ifdef HAS_SERIAL
  Serial.println("initialization done.");
#endif
  screen.stroke(255, 120, 120);
  screen.text("init SD card done.", 0, 16);
}

/**
 * сохраняем на SD
 */
void saveData(long time, float volt, float temp1, float hum1, float temp2, float hum2) {
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print("time=");
    dataFile.print(time / 60000.0, 2);
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
    //screen.drawFastVLine(curX, 16, 7, foneColor);
  } else {
#ifdef HAS_SERIAL
    Serial.println("error on datalog.txt");
#endif
/*
    fillPlace(0, 2, 27, screen.stroke(0, 0, 255));
    screen.print("error on datalog.txt", 0, 16);
*/
    //screen.drawFastVLine(curX, 16, 7, errorColor);
  }
}

/**
 * показываем на экране
 */
void showData(long time, float volt, float temp1, float hum1, float temp2, float hum2, bool isVline) {
  static int hour = 0;
  static int min = 0;
  uint16_t color = foneColor;
  if (isVline) {
    int tMin = time / 3600000; // 1 час 900000; // 15 мин
    if (min != tMin) {
      min = tMin;
      color = markMinColor;
    }
    int tHour = time / 108000000; // 3 часа
    if (hour != tHour) {
      hour = tHour;
      color = markHourColor;
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
void temperatureHumidity(float volt) {
  static int count = 0;
  static int cntX = 0;
  long   time  = millis();
  double temp1 = dht1.readTemperature();
  double hum1  = dht1.readHumidity();
  double temp2 = dht2.readTemperature();
  double hum2  = dht2.readHumidity();

  saveData(time, volt, temp1, hum1, temp2, hum2);

  if (count != time / TIME_MULTIPLIER) {
    count = time / TIME_MULTIPLIER;
    curX++; if (curX >= 144) curX = screenLeft;
    int curNextX = curX + 1; if (curNextX >= 144) curNextX = screenLeft;
    screen.drawFastVLine(curNextX, screenTop, 101, markColor);
    cntX = 0;
  } else {
    showData(time, volt, temp1, hum1, temp2, hum2, cntX == 0);
    cntX++;
  }
  fillPlace(17, 0, 8, colorTime);
  screen.print(time / 60000.0, 2);
  fillPlace(3, 0, 4, colorT1);
  screen.print(temp1, 1);
  fillPlace(12, 0, 4, colorB1);
  screen.print(hum1, 1);
  fillPlace(3, 1, 4, colorT2);
  screen.print(temp2, 1);
  fillPlace(12, 1, 4, colorB2);
  screen.print(hum2, 1);

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
