#include "ESP8266WebServer.h"
#include "HardwareSerial.h"
#include "SmartClockWeb8266.h"

void apiBright() {
  String json("{\"bright\":");
  json += analogRead(A0) * 3.3 / 1024;
  json += "}";
  server.enableCORS(true);
  server.send(200, "application/json", json);
}

void apiDatetime() {
  DateTime dateTime = Clock.now();
  String json("{\"year\": ");
  json += dateTime.year();
  json += ", \"month\": ";
  json += dateTime.month();
  json += ", \"day\": ";
  json += dateTime.day();
  json += ", \"hour\": ";
  json += dateTime.hour();
  json += ", \"minute\": ";
  json += dateTime.minute();
  json += ", \"second\": ";
  json += dateTime.second();
  json += "}";

  if (server.method() == HTTP_GET) {
    Serial.println("method == GET");
    server.enableCORS(true);
    server.send(200, "application/json", json);
  } else if (server.method() == HTTP_POST) {
    Serial.println("method == POST");
    String message = "  Form was:\n";
    for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
    Serial.println(message);
    Serial.println(server.argName(0));
    Serial.println(server.arg(0));

    server.enableCORS(true);
    server.send(200, "application/json", json);
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}
