#include "SmartClockWeb8266.h"

// инициализируем сервер на 80 порте
ESP8266WebServer server(80);
TFT_eSPI tft = TFT_eSPI();

void setup() {
  // Start the I2C interface
  Wire.begin(PIN_D1, PIN_D2);
  Serial.begin(115200);
  // инициализируем аналоговые пины
  pinMode(A0, INPUT);

  // Устанавливаем предварительное время
  //saveRealTime(new DateTime(2024, 2, 17, 6, 20, 26, 0));

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(1, 1, 319, 239, TFT_WHITE);
  //  tft.drawRect(CLOCK_X, CLOCK_Y, CLOCK_W, CLOCK_H, TFT_DARKGREEN);
  //  tft.drawRect(VOLT_X, VOLT_Y, VOLT_W, VOLT_H, TFT_BROWN);
  //testDrawFont();

  connectWiFi();
}

void loop() {
  if (checkWiFiConnected()) {
    server.handleClient();
  }
  outToTft();
  tftShowRealTime();
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

void testDrawFont() {
  tft.setTextFont(1);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  for (uint i = 0; i < 0x0100; i += 0x0020) {
    for (uint j = 0; j < 0x0020; j++) {
      tft.print((char)(i + j));
//      tft.print(" ");
    }
    tft.println();
  }
  tft.println("Съешь этих мягких булок");
  tft.println("АБВГДЕЖЗИЙКЛМНОП");
  tft.println("РСТУФХЦЧШЩЪЫЬЭЮЯ");
  tft.println("абвгдежзийклмноп");
  tft.println("рстуфхцчшщъыьэюяЁё");
  delay(100000);
}
