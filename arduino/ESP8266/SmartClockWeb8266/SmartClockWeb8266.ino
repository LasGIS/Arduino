#include "SmartClockWeb8266.h"

// инициализируем сервер на 80 порте
WiFiServer server(80);
TFT_eSPI tft = TFT_eSPI();
DS3231 Clock;

// переменная для хранения HTTP запроса
String header;

String bufTime("xx:xx:xx");
String bufDate("xx.xx.20xx");

// текущее время
unsigned long currentTime = millis();
// предыдущее время
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  // запускаем сервер
  server.begin();
  // Start the I2C interface
  Wire.begin(PIN_D1, PIN_D2);
  Serial.begin(115200);
  // инициализируем аналоговые пины
  pinMode(A0, INPUT);

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(1, 1, 319, 239, TFT_WHITE);
  //  tft.drawRect(CLOCK_X, CLOCK_Y, CLOCK_W, CLOCK_H, TFT_DARKGREEN);
  //  tft.drawRect(VOLT_X, VOLT_Y, VOLT_W, VOLT_H, TFT_BROWN);

  connectWiFi();
}

void loop() {
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
  outToTft();
  readDS3231();
  delay(100);
}

void outToTft() {
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK, true);
  tft.setViewport(VOLT_X, VOLT_Y, VOLT_W, VOLT_H);
  tft.setTextDatum(CC_DATUM);

  String strOut("Bright = ");
  strOut += analogRead(A0) * 3.3 / 1024;
  strOut += "V";
  tft.drawString(strOut, VOLT_W / 2, VOLT_H / 2, 4);
}

void readDS3231() {
  static long lastTime = 0L;
  long time = millis();
  if (lastTime != time / 1000) {
    DateTime dateTime = Clock.now();

    toTwoChar(dateTime.hour(), bufTime, 0);
    toTwoChar(dateTime.minute(), bufTime, 3);
    toTwoChar(dateTime.second(), bufTime, 6);

    toTwoChar(dateTime.day(), bufDate, 0);
    toTwoChar(dateTime.month(), bufDate, 3);
    toTwoChar(dateTime.year(), bufDate, 8);

    tft.setTextColor(TFT_GREENYELLOW, TFT_BLACK, true);
    tft.setViewport(CLOCK_X, CLOCK_Y, CLOCK_W, CLOCK_H);
    tft.setTextDatum(TC_DATUM);
    tft.setTextSize(3);
    tft.drawString(bufTime, CLOCK_W / 2, 0, 4);
    tft.setTextSize(1);
    tft.drawString(bufDate, CLOCK_W / 2, 80, 4);
    lastTime = time / 1000;
  }
}
