#include "SmartClockWeb8266.h"

// вводим имя и пароль точки доступа
const char* ssid     = "Your_SSID";
const char* password = "Your_Password";

void webRoute() {
  if (checkWiFiConnected()) {
    // анализируем канал связи на наличие входящих клиентов
    WiFiClient client = server.accept();
    if (client) {
      currentTime = millis();
      previousTime = currentTime;
      Serial.println("New Client.");
      String currentLine = "";
      while (client.connected() && currentTime - previousTime <= timeoutTime) {
        currentTime = millis();
        if (client.available()) {
          char c = client.read();
          // Serial.write(c);
          header += c;
          if (c == '\n') {
            if (currentLine.length() == 0) {
              Serial.println("header: {");
              Serial.print(header);
              Serial.println("}");
              if (header.indexOf("GET / ") >= 0) {
                // формируем основную веб-страницу
                webGetIndexHtml(client);
              } else if (header.indexOf("GET /static/styles.css ") >= 0) {
                webGetStylesCss(client);
              } else if (header.indexOf("GET /static/twocirclingarrows.svg ") >= 0) {
                webGetTwoCirclingArrowsSvg(client);
              } else if (header.indexOf("GET /src/common.js ") >= 0) {
                webGetSrcCommonJs(client);
              } else if (header.indexOf("GET /api/v1/bright ") >= 0) {
                webGetBright(client);
              } else if (header.indexOf("GET /api/v1/datetime ") >= 0) {
                webGetDatetime(client);
              } else if (header.indexOf("POST /api/v1/datetime ") >= 0) {
                webPostDatetime(client);
              }
              // выходим из цикла while
              break;
            } else {  // если появилась новая строка, очистим текущую строку
              currentLine = "";
            }
          } else if (c != '\r') {  // если у вас еще есть что то кроме символа возврата каретки,
            currentLine += c;      // добавляем его в конец текущей строки
          }
        }
      }
      // очищаем переменную заголовка
      header = "";
      // закрываем соединение
      client.stop();
      Serial.println("Client disconnected.");
    }
  }
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
