#include "SmartClockWeb8266.h"

// инициализируем сервер на 80 порте
WiFiServer server(80);
TFT_eSPI tft = TFT_eSPI();
DS3231 Clock;

String bufTime("xx:xx:xx");
String bufDate("xx.xx.20xx");

void setup() {
  // запускаем сервер
  server.begin();
  // Start the I2C interface
  Wire.begin(PIN_D1, PIN_D2);
  Serial.begin(115200);
  // инициализируем аналоговые пины
  pinMode(A0, INPUT);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(1, 1, 319, 239, TFT_WHITE);
  //  tft.drawRect(CLOCK_X, CLOCK_Y, CLOCK_W, CLOCK_H, TFT_DARKGREEN);
  //  tft.drawRect(VOLT_X, VOLT_Y, VOLT_W, VOLT_H, TFT_BROWN);

  connectWiFi();
}

void loop() {
  webLoop();
  outToTft();
  readDS3231();
  delay(100);
}

void outToTft() {
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK, true);
  tft.setViewport(VOLT_X, VOLT_Y, VOLT_W, VOLT_H);
  tft.setTextDatum(CC_DATUM);

  String strOut("Bright = ");
  strOut += analogRead(A0) * 3.3 / 1024;
  strOut += "V";
  tft.drawString(strOut, VOLT_W / 2, VOLT_H / 2, 4);
}

void readDS3231() {
  static long lastTime = 0L;
  long time = millis();
  if (lastTime != time / 1000) {
    DateTime dateTime = Clock.now();

    toTwoChar(dateTime.hour(), bufTime, 0);
    toTwoChar(dateTime.minute(), bufTime, 3);
    toTwoChar(dateTime.second(), bufTime, 6);

    toTwoChar(dateTime.day(), bufDate, 0);
    toTwoChar(dateTime.month(), bufDate, 3);
    toTwoChar(dateTime.year(), bufDate, 8);

    tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK, true);
    tft.setViewport(CLOCK_X, CLOCK_Y, CLOCK_W, CLOCK_H);
    tft.setTextDatum(TC_DATUM);
    tft.setTextSize(3);
    tft.drawString(bufTime, CLOCK_W / 2, 0, 4);
    tft.setTextSize(1);
    tft.drawString(bufDate, CLOCK_W / 2, 80, 4);
    lastTime = time / 1000;
  }
}
