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

#define TEMPERATURE_START 0.0
#define TEMPERATURE_MULTIPLIER 2.5

#define colorT1       0b0000000111011111
#define colorT2       0b0111100111011111
#define colorB1       0b0000011111100000
#define colorB2       0b0111111111100000
#define colorTime     0b1111100111100111
#define voltColor     0b1111100000011111
#define foneColor     0b0001000010000010
#define errorColor    0b0000000000011111
#define markColor     0b1111111111111111
#define markTempColor 0b1001010010010010
#define markMinColor  0b0110001100001100
#define markHourColor 0b1001010010010010

int curX = 0;

void setup() {
  // begin serial communication
  Serial.begin(9600);

  // Put this line at the beginning of every sketch that uses the GLCD:
  screen.begin(3);

  // clear the screen with a black background
  screen.background(0, 0, 0);

  screen.stroke(255, 255, 255);
  screen.setTextSize(1);
  //screen.text("Напряжение=10.00V\nТемпер.=    C Влажн.=    %", 0, 0);
  screen.text("Т1=    C В1=    %        м\nТ2=    C В2=    % V=     V", 0, 0);

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
    screen.drawFastVLine(curX, 16, 7, foneColor);
  } else {
    Serial.println("error on datalog.txt");
/*
    fillPlace(0, 2, 27, screen.stroke(0, 0, 255));
    screen.print("error on datalog.txt", 0, 16);
*/
    screen.drawFastVLine(curX, 16, 7, errorColor);
  }
}

/**
 * показываем на экране
 */
void showData(long time, float volt, float temp1, float hum1, float temp2, float hum2) {
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
  screen.drawFastVLine(curX, 27, 100, color);
  showTempMarks();
  if (!isnan(hum1)) showHum(hum1, colorB1);
  if (!isnan(hum2)) showHum(hum2, colorB2);
  if (!isnan(temp1)) showTemp(temp1, colorT1);
  if (!isnan(temp2)) showTemp(temp2, colorT2);
}

void showTempMarks() {
  for (float temp = TEMPERATURE_START; temp <= TEMPERATURE_START + 100 / TEMPERATURE_MULTIPLIER; temp += 10.0) {
    showTemp(temp, markTempColor);
  }
}

void showTemp(float temp, color col) {
  int ty = (int) (127 - (temp - TEMPERATURE_START) * TEMPERATURE_MULTIPLIER);
  screen.drawPixel(curX, ty, col);
}

void showHum(float hum, color col) {
  int hy = (int) (127 - hum);
  screen.drawPixel(curX, hy, col);
}

/**
 * Показываем температуру и влажность
 */
void temperatureHumidity(float volt) {
  static int count = -1;
  long   time  = millis();
  double temp1 = dht1.readTemperature();
  double hum1  = dht1.readHumidity();
  double temp2 = dht2.readTemperature();
  double hum2  = dht2.readHumidity();

  saveData(time, volt, temp1, hum1, temp2, hum2);

  if (count != time / 180000) {
    count = time / 180000;
    showData(time, volt, temp1, hum1, temp2, hum2);
    curX++; if (curX >= 160) curX = 0;
    screen.drawFastVLine(curX, 27, 101, markColor);
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
}
