#include <WiFi.h>
#include <SPI.h>
#include <TFT_eSPI.h>

// вводим имя и пароль точки доступа
const char* ssid     = "Your_SSID";
const char* password = "Your_Password";
// инициализируем сервер на 80 порте
WiFiServer server(80);
TFT_eSPI tft = TFT_eSPI();

#define VOLT_X 40
#define VOLT_W 240
#define VOLT_Y 80
#define VOLT_H 150

#define START_X 5
#define START_W 310
#define START_Y 3
#define START_H 234

char lineBuf[80];
int charCount = 0;

void setup() {
  Serial.begin(115200);
  // инициализируем аналоговые пины
  pinMode(36, INPUT);
  pinMode(39, INPUT);
  pinMode(34, INPUT);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // подключаем микроконтроллер к Wi-Fi сети
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wi-Fi connected");
  Serial.println("IP-address: ");
  Serial.println(WiFi.localIP());

  // запускаем сервер
  server.begin();

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.drawRect(1, 1, 319, 239, TFT_WHITE);

  //  tft.setTextWrap(true, true);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextFont(2);
  tft.setViewport(START_X, START_Y, START_W, START_H);

  tft.print("Connecting to ");
  tft.println(ssid);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("Wi-Fi connected");
  tft.print("IP-address: ");
  tft.println(WiFi.localIP().toString());
}

void loop() {
  // анализируем канал связи на наличие входящих клиентов
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    memset(lineBuf, 0, sizeof(lineBuf));
    charCount = 0;
    // HTTP-запрос заканчивается пустой строкой
    //        boolean currentLineIsBlank = true;
    while (client.connected()) {
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      // формируем веб-страницу
      String webPage = "<!DOCTYPE HTML>";
      webPage += "<html lang=\"en\">";
      webPage += "  <head>";
      webPage += "    <meta charset=\"utf-8\" />";
      webPage += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
      webPage += "  </head>";
      webPage += "  <h1>ESP32 - Web Server (по Русски)</h1>";
      webPage += "  <p>";
      webPage += "  AnalogPin 36 = ";
      webPage += analogRead(36) * 3.3 / 4095;
      webPage += " V<br>";
      webPage += "  AnalogPin 39 = ";
      webPage += analogRead(39) * 3.3 / 4095;
      webPage += " V<br>";
      webPage += "  AnalogPin 34 = ";
      webPage += analogRead(34) * 3.3 / 4095;
      webPage += " V<br>";
      webPage += "  </p>";
      webPage += "</html>";
      client.println(webPage);
      break;
    }
    // даем веб-браузеру время для получения данных
    delay(1);
    // закрываем соединение
    client.stop();
    Serial.println("client disconnected");
  }
  outToTft();
  delay(100);
}

void outToTft() {
  tft.setTextColor(TFT_MAGENTA, TFT_DARKGREY, true);
  tft.setViewport(VOLT_X, VOLT_Y, VOLT_W, VOLT_H);
  tft.setCursor(0, 0, 4);

  String strOut = "AnalogPin 36 = ";
  strOut += analogRead(36) * 3.3 / 4095;
  strOut += "V";
  //  Serial.println(strOut);
  tft.println(strOut);
  strOut = "AnalogPin 39 = ";
  strOut += analogRead(39) * 3.3 / 4095;
  strOut += "V";
  //  Serial.println(strOut);
  tft.println(strOut);
  strOut = "AnalogPin 34 = ";
  strOut += analogRead(34) * 3.3 / 4095;
  strOut += "V";
  //  Serial.println(strOut);
  tft.println(strOut);
}
