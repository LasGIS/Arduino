#include "SmartClockWeb8266.h"

void apiBright() {
  String json("{\"bright\":");
  json += analogRead(A0) * 3.3 / 1024;
  json += "}";
  server.enableCORS(true);
  server.send(200, "application/json", json);
}

String toJson(DateTime * dateTime) {
  String json("{\"year\": ");
  json += dateTime->year();
  json += ", \"month\": ";
  json += dateTime->month();
  json += ", \"day\": ";
  json += dateTime->day();
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

    //    Serial.print("Content-Type == ");
    //    Serial.println(server.header("Content-Type"));
    /*
    String message = "  Form was:\n";
    for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
    Serial.println(message);
    Serial.println(server.argName(0));
    Serial.println(server.arg(0));
*/

    String inJson;
    if (server.hasArg("plain")) {
      inJson = server.arg("plain");
    } else {
      inJson = server.argName(0);
    }
    Serial.println("json = " + inJson);
    JSONVar objJson = JSON.parse(inJson);
    DateTime * dateTime = new DateTime(
      objJson["year"], objJson["month"], objJson["day"], 0,
      objJson["hour"], objJson["minute"], objJson["second"]);
    saveRealTime(dateTime);

    server.enableCORS(true);
    server.send(200, "application/json", toJson(dateTime));
  } else {
    server.send(405, "text/plain", "Method Not Allowed");
  }
}
