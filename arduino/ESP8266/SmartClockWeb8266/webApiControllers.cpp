#include "SmartClockWeb8266.h"

void apiBright() {
  String json("{\"bright\":");
  json += analogRead(A0) * 3.3 / 1024;
  json += "}";
  server.enableCORS(true);
  server.send(200, "application/json", json);
}

String toJson(DateTime *dateTime) {
  String json("{\"year\": ");
  json += dateTime->year();
  json += ", \"month\": ";
  json += dateTime->month();
  json += ", \"day\": ";
  json += dateTime->day();
  json += ", \"dayOfWeek\": ";
  json += dateTime->dayOfWeek();
  json += ", \"hour\": ";
  json += dateTime->hour();
  json += ", \"minute\": ";
  json += dateTime->minute();
  json += ", \"second\": ";
  json += dateTime->second();
  json += "}";
  return json;
}

void apiDatetime() {
  if (server.method() == HTTP_GET) {
    Serial.println("method == GET");
    server.enableCORS(true);
    DateTime dateTime = Clock.now();
    server.send(200, "application/json", toJson(&dateTime));
  } else if (server.method() == HTTP_POST) {
    Serial.println("method == POST");
    String inJson;
    if (server.hasArg("plain")) {
      inJson = server.arg("plain");
    } else {
      inJson = server.argName(0);
    }
    Serial.println("json = " + inJson);
    Json objJson(inJson);
    DateTime *dateTime = new DateTime(
      objJson["year"].as<uint16_t>(),
      objJson["month"].as<uint8_t>(),
      objJson["day"].as<uint8_t>(),
      objJson["dayOfWeek"].as<uint8_t>(),
      objJson["hour"].as<uint8_t>(),
      objJson["minute"].as<uint8_t>(),
      objJson["second"].as<uint8_t>()
    );
    saveRealTime(dateTime);

    server.enableCORS(true);
    server.send(200, "application/json", toJson(dateTime));
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}

void scanNetworks() {
  if (server.method() == HTTP_GET) {
    int numberOfNetworks = WiFi.scanNetworks();
    JsonArray jsonArray;
    for (int i = 0; i < numberOfNetworks; i++) {
      Json jsonVar;
      Serial.print("Network name: ");
      Serial.println(WiFi.SSID(i));
      jsonVar.add("name", WiFi.SSID(i));
      Serial.print("Signal strength: ");
      Serial.println(WiFi.RSSI(i));
      jsonVar.add("rssi", WiFi.RSSI(i));
      Serial.println("-----------------------");
      jsonArray.push(jsonVar);
      Serial.println(jsonVar.toString());
    }
    Json jsonObject;
    jsonObject["count"] = numberOfNetworks;
    jsonObject["ssids"] = jsonArray;
    server.enableCORS(true);
    server.send(200, "application/json", jsonObject.toString());
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}
