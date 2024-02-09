#include <WiFi.h>

static const char indexHtml1[] = R"=====(
<!DOCTYPE HTML>
<html lang="en"><head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <link rel="icon" href="data:,">
</head><body>
  <h1>Web Server (по Русски)</h1>
<p>Яркость =
)=====";

static const char indexHtml2[] = R"=====(
 V</p>
</body></html>
)=====";

void webOutIndexHtml(WiFiClient * client) {
  client->print(indexHtml1);
  client->print(analogRead(34) * 3.3 / 4095);
  client->print(indexHtml2);
}
