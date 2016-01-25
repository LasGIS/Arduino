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

File myFile;

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

// char array to print to the screen
char sensorPrintout[5];

void setup() {
  // begin serial communication
  Serial.begin(9600);

  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255, 255, 255);
  // set the font size
  TFTscreen.setTextSize(1);
  // write the text to the top left corner of the screen
  TFTscreen.text("Sensor Value:\n абвгдеёжз", 0, 0);
  initSD();
  // ste the font size very large for the loop
  TFTscreen.setTextSize(4);
  
}

void loop() {

  // Read the value of the sensor on A0
  int sensor = analogRead(A0);
  String sensorVal = String(sensor);
  // convert the reading to a char array
  sensorVal.toCharArray(sensorPrintout, 5);

  // set the font color
  TFTscreen.stroke(255, 0, 255);
  // print the sensor value
  TFTscreen.text(sensorPrintout, 0, 20);
  //Serial.println(sensor);
  saveData(sensor);
  // wait for a moment
  delay(250);
  // erase the text you just wrote
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.text(sensorPrintout, 0, 20);
}

void initSD() {
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization SD card failed!");
    TFTscreen.text("init SD card failed!", 0, 50);
    return;
  }
  Serial.println("initialization done.");
  TFTscreen.text("init SD card done.\n", 0, 50);
 
  if (SD.exists("example.txt")) {
    Serial.println("example.txt exists.");
  } else {
    Serial.println("example.txt doesn't exist.");
  }
 
  //Откроем новый файл и сразу же закроем его:
  Serial.println("Creating example.txt...");
  myFile = SD.open("example.txt", FILE_WRITE);
  myFile.close();
 
  //Проверяем, создан ли файл:
  if (SD.exists("example.txt")) {
    Serial.println("example.txt exists.");
    TFTscreen.println("example.txt exists.");
  } else {
    Serial.println("example.txt doesn't exist.");
    TFTscreen.println("example.txt doesn't exist.");
  }
}

void saveData(int sensor) {
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(sensor);
    dataFile.close();
  } else {
    Serial.println("error on datalog.txt");
  }
}

