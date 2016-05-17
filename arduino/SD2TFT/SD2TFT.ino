/**
 * Читаем MicroSD на TFT Экран
 * SD карта подключается так:
 ** MOSI - пин 11
 ** MISO - пин 12
 ** CLK - пин 13
 ** CS - пин 4
 */
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
TFT TFTscreen = TFT(cs, dc, rst);

/** настраиваем измеритель влажности. */
DHT dht(6, DHT22);

void setup() {
  // begin serial communication
  Serial.begin(9600);

  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin(3);

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
//  TFTscreen.drawRect(0, 28, 160, 100, 0xf00f);
  // set the font size
  TFTscreen.setTextSize(1);
  // write the text to the top left corner of the screen
  TFTscreen.text("Напряжение=10.00V\nТемпер.=    C Влажн.=    %", 0, 0);
  initSD();
  dht.begin();
}

void loop() {
  static int count = 0;

  // Read the value of the sensor
  float factor = 2.5 / analogRead(A7);
  float sensor = analogRead(A6) * factor;

  if (count % 2 == 0) {
    temperatureHumidity(sensor);
    count = 0;
  }

  TFTscreen.stroke(255, 0, 255);
  TFTscreen.fillRect(66, 0, 29, 7, 0);
  TFTscreen.text("", 66, 0);
  TFTscreen.print(sensor, 2);

  delay(1000);
  count++;
}

/**
 * Инициализация SD
 */
void initSD() {
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    TFTscreen.stroke(0, 0, 255);
    Serial.println("initialization SD card failed!");
    TFTscreen.text("init SD card failed!", 0, 16);
    return;
  }
  Serial.println("initialization done.");
  TFTscreen.stroke(255, 120, 120);
  TFTscreen.text("init SD card done.", 0, 16);
}

/**
 * сохраняем на SD
 */
void saveData(float volt, float temp, float hum) {
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print("T=");
    dataFile.print(temp, 1);
    dataFile.print(",H=");
    dataFile.print(hum, 1);
    dataFile.print(",V=");
    dataFile.println(volt, 2);
    dataFile.close();
  } else {
    Serial.println("error on datalog.txt");
    TFTscreen.stroke(0, 0, 255);
    TFTscreen.fillRect(0, 16, 160, 7, 0);
    TFTscreen.text("error on datalog.txt", 0, 16);
  }
}

/**
 * показываем на экране
 */
void showData(float volt, float temp, float hum) {
  static int x = 0;
  if (x >= 160) x = 0;
  int ty = (int) (128 - (temp - 20) * 10);
  int hy = (int) (128 - hum);
  TFTscreen.drawFastVLine(x, 28, 100, TFTscreen.newColor(20, 20, 20));
  TFTscreen.drawPixel(x, ty, TFTscreen.newColor(0, 130, 255));
  TFTscreen.drawPixel(x, hy, TFTscreen.newColor(0, 255, 0));
  x++;
}

/**
 * Показываем температуру и влажность
 */
void temperatureHumidity(float volt) {
  double hum = dht.readHumidity();
  double temp = dht.readTemperature();
  saveData(volt, temp, hum);
  showData(volt, temp, hum);

  TFTscreen.stroke(0, 120, 255);
  TFTscreen.fillRect(48, 8, 23, 7, 0);
  TFTscreen.text("", 48, 8);
  TFTscreen.print(temp, 1);

  TFTscreen.stroke(0, 255, 0);
  TFTscreen.fillRect(126, 8, 23, 7, 0);
  TFTscreen.text("", 126, 8);
  TFTscreen.print(hum, 1);

  Serial.print("Volt: ");
  Serial.print(volt);
  Serial.print(" V\tTemperature: ");
  Serial.print(temp);
  Serial.print(" *C\tHumidity: ");
  Serial.print(hum);
  Serial.println(" %");
}
