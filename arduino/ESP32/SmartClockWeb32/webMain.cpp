#include "SmartClockWeb32.h"

// Define timeout time in milliseconds (example: 2000ms = 2s)
#define TIME_OUT_TIME 2000

// вводим имя и пароль точки доступа
//const char* ssid =     "IT_One Guest";
//const char* password = "IT-One2020";
const char* ssid =     "TP-Link_C857";
const char* password = "14178054";
//const char* ssid =     "POCO M4 Pro";
//const char* password = "5cc2crfftk4ur6v";

void connectWiFi();
bool checkWiFiConnected();

// текущее время
unsigned long currentTime = millis();
// предыдущее время
unsigned long previousTime = 0;

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
  // запускаем сервер
  server.begin();
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
    }
    lastTime = time / 500;
  }
  return isConnected;
}

void handleClient() {
  // переменная для хранения HTTP запроса
  String header;

  // анализируем канал связи на наличие входящих клиентов
  WiFiClient client = server.available();
  if (client) {
    currentTime = millis();
    previousTime = currentTime;
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

            Serial.println("header: {");
            Serial.print(header);
            Serial.println("}");

            client.println("HTTP/1.1 200 OK");
            client.println("Content-type: text/html");
            client.println("Connection: close");
            client.println();
            // формируем веб-страницу
            webOutIndexHtml(&client);
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
