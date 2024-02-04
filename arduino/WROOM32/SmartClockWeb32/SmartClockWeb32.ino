#include <WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>
#include <TFT_eSPI.h>

// вводим имя и пароль точки доступа
const char* ssid = "Your_SSID";
const char* password = "Your_Password";
// инициализируем сервер на 80 порте
WiFiServer server(80);
TFT_eSPI tft = TFT_eSPI();
DS3231 Clock;

String bufTime("xx:xx:xx");
String bufDate("xx.xx.20xx");

#define START_X 5
#define START_W 310
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

char lineBuf[80];
int charCount = 0;

void setup() {
  // Start the I2C interface
  Wire.begin();
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
//  tft.drawRect(CLOCK_X, CLOCK_Y, CLOCK_W, CLOCK_H, TFT_DARKGREEN);
//  tft.drawRect(VOLT_X, VOLT_Y, VOLT_W, VOLT_H, TFT_BROWN);

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
      String webPage =
        "<!DOCTYPE HTML>"
        "<html lang=\"en\">"
        "<head>"
        "<meta charset=\"utf-8\" />"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
        "</head>"
        "<body>"
        "<h1>Web Server (по Русски)</h1>"
        "<p>Яркость = ";
      webPage += analogRead(34) * 3.3 / 4095;
      webPage +=
        " V</p>"
        "</body>"
        "</html>";
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
  readDS3231();
  delay(100);
}

void outToTft() {
  tft.setTextColor(TFT_MAGENTA, TFT_BLACK, true);
  tft.setViewport(VOLT_X, VOLT_Y, VOLT_W, VOLT_H);
  tft.setTextDatum(CC_DATUM);

  String strOut("Bright = ");
  strOut += analogRead(34) * 3.3 / 4095;
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
