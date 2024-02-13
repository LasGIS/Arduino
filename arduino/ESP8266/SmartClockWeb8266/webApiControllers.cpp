#include "HardwareSerial.h"
#include "SmartClockWeb8266.h"

void webGetBright(WiFiClient& client) {
  String json("{\"bright\":");
  json += analogRead(A0) * 3.3 / 1024;
  json += "}";
  client.print(R"=====(
HTTP/1.0 200 OK
Content-type: application/json
Access-Control-Allow-Origin: *
Content-Length: )=====");
  client.println(json.length());
  client.println();
  client.print(json);
}

void webGetDatetime(WiFiClient& client) {
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

  client.print(R"=====(
HTTP/1.0 200 OK
Accept: application/json
Content-type: application/json
Access-Control-Allow-Origin: *
Content-Length: )=====");
  client.println(json.length());
  client.println();
  client.print(json);
}

void webPostDatetime(WiFiClient& client) {
  String content = webLoadContent(client, 77);
  Serial.println(content);
  
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

  client.print(R"=====(
HTTP/1.0 200 OK
Accept: application/json
Content-type: application/json
Access-Control-Allow-Origin: *
Content-Length: )=====");
  client.println(json.length());
  client.println();
  client.print(json);
}
