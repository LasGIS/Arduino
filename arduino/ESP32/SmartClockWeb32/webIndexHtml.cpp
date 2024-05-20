#include "SmartClockWeb32.h"

const char indexHtml[] = R"=====(<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <link rel="icon" href="data:," />
  <link type="text/css" href="static/styles.css" rel="stylesheet" />
  <script type="text/javascript" src="src/common.js"></script>
  <title>Умные часы ESP-32</title>
</head>
<body onload="{getBright();getDatetime();}">
  <h1>Умные часы ESP-32</h1>
  <p>Яркость = <span id="bright"></span> V
    <abbr onclick="getBright()" class="icon icon-synchronize link"></abbr>
  </p>
  <p>Время = <span id="control-time"></span>
    <abbr onclick="getDatetime()" class="icon icon-synchronize link"></abbr>
  </p>
  <p>Дата = <span id="control-data"></span></p>
  <p><label for="day-of-week">День недели</label>
    <select id="day-of-week">
      <option value="0">Воскресенье</option>
      <option value="1">Понедельник</option>
      <option value="2">Вторник</option>
      <option value="3">Среда</option>
      <option value="4">Четверг</option>
      <option value="5">Пятница</option>
      <option value="6">Суббота</option>
    </select>
    <abbr onclick="synchroDatetime()" class="icon icon-synchronize link"
      title="Синхронизировать часы со значением на компьютере"></abbr>
  </p>
  <div>
    <a href="/"><button class="button">Обновить</button></a>
    <a href="#" onclick="synchroDatetime()"><button class="button">Синхронизировать</button></a>
    <a href="#" onclick="showScanNetworks()"><button class="button">Сканировать WiFi</button></a>
  </div>
  <div>
    <textarea cols="108" rows="20" id="scan-wifi"></textarea>
  </div>
</body>
</html>
)=====";

void webGetIndexHtml() {
  server.send(200, "text/html", indexHtml);
}
