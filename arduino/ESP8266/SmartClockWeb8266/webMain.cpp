#include "SmartClockWeb8266.h"

// Define timeout time in milliseconds (example: 2000ms = 2s)
#define TIME_OUT_TIME 2000

// вводим имя и пароль точки доступа
const char* ssid     = "Your_SSID";
const char* password = "Your_Password";

void webLoop() {
  if (checkWiFiConnected()) {
    // анализируем канал связи на наличие входящих клиентов
    WiFiClient client = server.accept();
    if (client) {
      // текущее время
      unsigned long currentTime = millis();
      // предыдущее время
      unsigned long previousTime = currentTime;
      // переменная для хранения HTTP запроса
      String header;

      Serial.println("New Client.");
      String currentLine = "";
      while (client.connected() && currentTime - previousTime <= TIME_OUT_TIME) {
        currentTime = millis();
        if (client.available()) {
          char c = client.read();
          // Serial.write(c);
          header += c;
          if (c == '\n') {
            if (currentLine.length() == 0) {
              webRoute(client, header);
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

String webLoadContent(WiFiClient& client, unsigned int count) {
  unsigned long currentTime = millis();
  unsigned long previousTime = currentTime;
  char* buf = new char[count + 1];
  unsigned int pos = 0;
  while (client.connected() && currentTime - previousTime <= TIME_OUT_TIME && pos <= count) {
    currentTime = millis();
    if (client.available()) {
      pos += client.read(buf + pos, count - pos);
    }
  }
  buf[pos] = 0;
  return String(buf);
}

void webRoute(WiFiClient& client, String header) {
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
