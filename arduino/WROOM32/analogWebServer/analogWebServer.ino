// подключяем библиотеку для работы с Wi-Fi server
#include <WiFi.h>
// вводим имя и пароль точки доступа
const char* ssid     = "Your_SSID";
const char* password = "Your_Password";
// инициализируем сервер на 80 порте
WiFiServer server(80);
// заводим буфер и счетчик для буфера
char lineBuf[80];
int charCount = 0;

void setup() {
    // инициализируем монитор порта
    Serial.begin(115200);
    // запас времени на открытие монитора порта — 5 секунд
    delay(5000);
    // инициализируем аналоговые пины
    pinMode(36, INPUT);
    pinMode(39, INPUT);
    pinMode(34, INPUT);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    // подключаем микроконтроллер к Wi-Fi сети
    WiFi.begin(ssid, password);
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Wi-Fi connected");
    Serial.println("IP-address: ");
    Serial.println(WiFi.localIP());
    // запускаем сервер
    server.begin();
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
            webPage += "<html>";
            webPage += "  <head>";
            webPage += "    <meta name=\"viewport\" content=\"width=device-width,";
            webPage += "    initial-scale=1\">";
            webPage += "  </head>";
            webPage += "  <h1>ESP32 - Web Server (по Русски)</h1>";
            webPage += "  <p>";
            webPage += "  	AnalogPin 36 = ";
            webPage += 		analogRead(36);
            webPage += "	<br>";
            webPage += "  	AnalogPin 39 = ";
            webPage += 		analogRead(39);
            webPage += "	<br>";
            webPage += "  	AnalogPin 34 = ";
            webPage += 		analogRead(34);
            webPage += "	<br>";
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
}
