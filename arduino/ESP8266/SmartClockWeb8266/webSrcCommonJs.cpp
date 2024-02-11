#include "SmartClockWeb8266.h"

const char srcCommonJs[] = R"=====(// Common JS 
let bright = 3.3;
const setBright = (volt) => {
  bright = volt;
  const elm = document.getElementById("bright");
  elm.textContent = bright.toFixed(2);
};
)=====";

void webOutSrcCommonJs(WiFiClient& client) {
  client.print(R"=====(
HTTP/1.0 200 OK
Content-type: application/javascript
Content-Length: )=====");
  client.println(strlen(srcCommonJs));
  client.println();
  client.print(srcCommonJs);
}
