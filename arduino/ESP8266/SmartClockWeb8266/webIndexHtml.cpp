#include "SmartClockWeb8266.h"

const char indexHtml1[] = R"=====(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <link rel="icon" href="data:," />
  <link type="text/css" href="static/styles.css" rel="stylesheet" />
  <script type="text/javascript" src="src/common.js"></script>
  <title>Умные часы ESP8266</title>
</head>
<body onload="setBright()=====";

const char indexHtml2[] = R"=====()">
  <h1>Умные часы ESP8266</h1>
  <p>Яркость = <span id="bright"></span> V<abbr onclick="setBright(3.1)" class="icon icon-synchronize link"></abbr></p>
  <a href="/"><button class="button">Обновить</button></a>
</body>
</html>
)=====";

void webGetIndexHtml(WiFiClient& client) {
  client.print(R"=====(
HTTP/1.1 200 OK
Content-type: text/html
Connection: close
)=====");
  client.println();
  client.print(indexHtml1);
  client.print(analogRead(34) * 3.3 / 1024);
  client.print(indexHtml2);
}
