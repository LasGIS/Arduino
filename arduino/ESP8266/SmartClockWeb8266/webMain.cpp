#include "SmartClockWeb8266.h"

// вводим имя и пароль точки доступа
const char* ssid     = "Your_SSID";
const char* password = "Your_Password";

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
        isConnected = true;
      } else {
        Serial.print(".");
      }
      // } else {
      // isConnected = WiFi.status() == WL_CONNECTED;
    }
    lastTime = time / 500;
  }
  return isConnected;
}
