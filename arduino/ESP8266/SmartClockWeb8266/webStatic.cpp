#include "SmartClockWeb8266.h"

const char stylesCss[] = R"=====(
html {
  font-family: Helvetica, sans-serif;
  display: inline-block;
  margin: 0 auto;
  text-align: center;
}
p > span {
  color: #002288;
  font-weight: bolder;
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
.button2 {
  background-color: #555555;
}
)=====";

void webGetStylesCss(WiFiClient& client) {
  client.print(R"=====(
HTTP/1.0 200 OK
Content-type: text/css
Content-Length: )=====");
  client.println(strlen(stylesCss));
  client.println();
  client.print(stylesCss);
}
