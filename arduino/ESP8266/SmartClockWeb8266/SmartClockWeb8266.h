#ifndef SMART_CLOCK_WEB_8266_H
#define SMART_CLOCK_WEB_8266_H

#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>
#include <TFT_eSPI.h>

extern WiFiServer server;
extern TFT_eSPI tft;
extern DS3231 Clock;

#define START_X 10
#define START_W 300
#define START_Y 3
#define START_H 234

#define CLOCK_X 5
#define CLOCK_W 310
#define CLOCK_Y 70
#define CLOCK_H 100

#define VOLT_X 40
#define VOLT_W 240
#define VOLT_Y 180
#define VOLT_H 50

/** общие переменные */
extern String bufTime;
extern String bufDate;

/** общие функции */
extern void toTwoChar(int val, String& str, unsigned int start);

extern void webLoop();
extern void webRoute(WiFiClient& client, String header);
extern void connectWiFi();
extern bool checkWiFiConnected();
extern String webLoadContent(WiFiClient& client, unsigned int count);
extern void webGetIndexHtml(WiFiClient& client);
extern void webGetStylesCss(WiFiClient& client);
extern void webGetTwoCirclingArrowsSvg(WiFiClient& client);
extern void webGetSrcCommonJs(WiFiClient& client);
extern void webGetBright(WiFiClient& client);
extern void webGetDatetime(WiFiClient& client);
extern void webPostDatetime(WiFiClient& client);

#endif // SMART_CLOCK_WEB_8266_H
