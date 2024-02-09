#include <WiFi.h>

const char indexHtml1[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <link rel="icon" href="data:," />
  <link type="text/css" href="static/styles.css" rel="stylesheet" />
  <style>
  html {
    font-family: Helvetica, sans-serif;
    display: inline-block;
    margin: 0 auto;
    text-align: center;
  }
  .button {
    background-color: #4CAF50;
    border: none;
    color: white;
    padding: 16px 40px;
    text-decoration: none;
    font-size: 30px;
    margin: 2px;
    cursor: pointer;
  }
  .icon {
    display: inline-block;
    vertical-align: text-bottom;
    margin-left: 10px;
    width: 22px;
    height: 20px;
    background-color: #DC143C80;
    mask-size: 22px;
  }
  abbr.icon.icon-deprecated {
    mask-image: url(deprecated.svg);
  }
  abbr.icon.link {
    cursor: pointer;
  }
  abbr.icon.link:hover {
    background-color: #DC143CFF;
  }
  abbr.icon.icon-synchronize {
    mask-image: url(twocirclingarrows.svg);
  }
  </style>
  <script>
let bright = 3.3;
const setBright = (volt) => {
  bright = volt;
  const elm = document.getElementById("bright");
  elm.textContent = bright.toFixed(2);
};
  </script>
  <title>Умные часы ESP-32</title>
</head>
<body onload="setBright(
)=====";

const char indexHtml2[] PROGMEM = R"=====(
)">
  <h1>Умные часы ESP-32</h1>
  <p>Яркость = <span id="bright"></span> V<abbr onclick="setBright(3.1)" class="icon icon-synchronize link"></abbr></p>
  <a href="/"><button class="button">Обновить</button></a>
</body>
</html>
)=====";

void webOutIndexHtml(WiFiClient * client) {
  client->print(indexHtml1);
  client->print(analogRead(34) * 3.3 / 4095);
  client->print(indexHtml2);
}
