/**
 * Читаем MicroSD на TFT Экран
 * SD карта подключается так:
 ** MOSI - пин 11
 ** MISO - пин 12
 ** CLK - пин 13
 ** CS - пин 4
 */
#include <Arduino.h>
#include <TFT_lg.h>
#include <SPI.h>
#include <SD.h>;
#include <DHT.h>

File myFile;

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// create an instance of the library
TFT screen = TFT(cs, dc, rst);

/** настраиваем измеритель влажности. */
DHT dht(6, DHT22);

uint16_t foneColor;
uint16_t errorColor;
uint16_t markColor;
uint16_t markHourColor;
uint16_t markMinColor;
int curX = 0;

void setup() {
  // begin serial communication
  Serial.begin(9600);

  // Put this line at the beginning of every sketch that uses the GLCD:
  screen.begin(3);

  // clear the screen with a black background
  screen.background(0, 0, 0);
  foneColor = screen.newColor(50, 50, 50);
  errorColor = screen.newColor(0, 0, 255);
  markColor = screen.newColor(255, 255, 255);
  markMinColor = screen.newColor(150, 100, 100);
  markHourColor = screen.newColor(150, 150, 150);

  screen.stroke(255, 255, 255);
  screen.setTextSize(1);
  screen.text("Напряжение=10.00V\nТемпер.=    C Влажн.=    %", 0, 0);

  initSD();

  dht.begin();
}

void loop() {
  // Read the value of the sensor
  float factor = 2.5 / analogRead(A7);
  float sensor = analogRead(A6) * factor;

  temperatureHumidity(sensor);

  screen.stroke(255, 0, 255);
  screen.fillRect(66, 0, 29, 7, 0);
  screen.text("", 66, 0);
  screen.print(sensor, 2);

  delay(2000);
}

/**
 * Инициализация SD
 */
void initSD() {
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    screen.stroke(0, 0, 255);
    Serial.println("initialization SD card failed!");
    screen.text("init SD card failed!", 0, 16);
    return;
  }
  Serial.println("initialization done.");
  screen.stroke(255, 120, 120);
  screen.text("init SD card done.", 0, 16);
}

/**
 * сохраняем на SD
 */
void saveData(long time, float volt, float temp, float hum) {
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print("time=");
    dataFile.print(time / 60000.0, 2);
    dataFile.print(",T=");
    dataFile.print(temp, 1);
    dataFile.print(",H=");
    dataFile.print(hum, 1);
    dataFile.print(",V=");
    dataFile.println(volt, 2);
    dataFile.close();
    screen.drawFastVLine(curX, 16, 7, foneColor);
  } else {
    Serial.println("error on datalog.txt");
/*
    screen.stroke(0, 0, 255);
    screen.fillRect(0, 16, 160, 7, 0);
    screen.text("error on datalog.txt", 0, 16);
*/
    screen.drawFastVLine(curX, 16, 7, errorColor);
  }
}

/**
 * показываем на экране
 */
void showData(long time, float volt, float temp, float hum) {
  static int hour = 0;
  static int min = 0;
  int tHour = time / 3600000;
  int tMin = time / 900000; // 15 мин
  uint16_t color = foneColor;
  if (min != tMin) {
    min = tMin;
    color = markMinColor;
  }
  if (hour != tHour) {
    hour = tHour;
    color = markHourColor;
  }
  int ty = (int) (128 - (temp - 20) * 10);
  int hy = (int) (128 - hum);
  screen.drawFastVLine(curX, 28, 100, color);
  screen.drawPixel(curX, ty, screen.newColor(0, 130, 255));
  screen.drawPixel(curX, hy, screen.newColor(0, 255, 0));
}

/**
 * Показываем температуру и влажность
 */
void temperatureHumidity(float volt) {
  static int count = -1;
  long time = millis();
  double hum = dht.readHumidity();
  double temp = dht.readTemperature();

  saveData(time, volt, temp, hum);

  if (count != time / 180000) {
    count = time / 180000;
    showData(time, volt, temp, hum);
    curX++; if (curX >= 160) curX = 0;
    screen.drawFastVLine(curX, 28, 100, markColor);
  }
  screen.stroke(0, 120, 255);
  screen.fillRect(48, 8, 23, 7, 0);
  screen.text("", 48, 8);
  screen.print(temp, 1);

  screen.stroke(0, 255, 0);
  screen.fillRect(126, 8, 23, 7, 0);
  screen.text("", 126, 8);
  screen.print(hum, 1);

  Serial.print("Volt: ");
  Serial.print(volt);
  Serial.print(" V\tTemperature: ");
  Serial.print(temp);
  Serial.print(" *C\tHumidity: ");
  Serial.print(hum);
  Serial.println(" %");
}
