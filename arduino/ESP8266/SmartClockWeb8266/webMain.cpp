#include "SmartClockWeb8266.h"

// Define timeout time in milliseconds (example: 2000ms = 2s)
#define TIME_OUT_TIME 2000

// вводим имя и пароль точки доступа
const char* ssid =     "IT_One Guest";
const char* password = "IT-One2020";
//const char* ssid =     "TP-Link_C857";
//const char* password = "14178054";
//const char* ssid =     "POCO M4 Pro";
//const char* password = "5cc2crfftk4ur6v";

void webRoute() {
  // формируем основную веб-страницу
  server.on("/", webGetIndexHtml);
  server.on("/static/styles.css", webGetStylesCss);
  server.on("/static/twocirclingarrows.svg", webGetTwoCirclingArrowsSvg);
  server.on("/static/deprecated.svg", webGetDeprecatedSvg);
  server.on("/src/common.js", webGetSrcCommonJs);

  // API
  server.on("/api/v1/bright", apiBright);
  server.on("/api/v1/datetime", apiDatetime);

  server.begin();
}

void connectWiFi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  tft.setTextFont(2);
  tft.setViewport(START_X, START_Y, START_W, START_H);
  tft.setTextColor(TFT_DARKCYAN, TFT_BLACK);
  tft.print("Connecting to ");
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.println(ssid);
  // подключаем микроконтроллер к Wi-Fi сети
  WiFi.begin(ssid, password);
}

bool checkWiFiConnected() {
  static bool isConnected = false;
  static long lastTime = 0L;
  long time = millis();
  if (lastTime != time / 500) {
    if (!isConnected) {
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println();
        Serial.println("Wi-Fi connected");
        Serial.println("IP-address: ");
        Serial.println(WiFi.localIP());
        tft.setTextFont(2);
        tft.setViewport(START_X, START_Y, START_W, START_H);
        tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
        tft.print("Wi-Fi Host: ");
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.println(WiFi.localIP().toString());
        webRoute();
        isConnected = true;
      } else {
        Serial.print(".");
      }
    }
    lastTime = time / 500;
  }
  return isConnected;
}
