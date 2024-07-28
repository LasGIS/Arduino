#include "SmartClockWeb32.h"

// инициализируем сервер на 80 порте
WebServer server(80);
TFT_eSPI tft = TFT_eSPI();

void setup() {
  // Start the I2C interface
  Wire.begin();
  Serial.begin(115200);
  // инициализируем аналоговые пины
  pinMode(34, INPUT);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(1, 1, 319, 239, TFT_WHITE);
  //  tft.drawRect(CLOCK_X, CLOCK_Y, CLOCK_W, CLOCK_H, TFT_DARKGREEN);
  //  tft.drawRect(VOLT_X, VOLT_Y, VOLT_W, VOLT_H, TFT_BROWN);

  // проверка Little FS
  littleFsTest();

  // запускаем сервер
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
  strOut += analogRead(34) * 3.3 / 4095;
  strOut += "V";
  tft.drawString(strOut, VOLT_W / 2, VOLT_H / 2, 4);
}

void littleFsTest() {
  
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  
  File file = LittleFS.open("/index.html", "r");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  
  Serial.println("File Content:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}
