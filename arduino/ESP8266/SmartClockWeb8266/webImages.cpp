#include "SmartClockWeb8266.h"

const char twoCirclingArrowsSvg[] = R"=====(
<svg version="1.1" id="Capa_1" xmlns="http://www.w3.org/2000/svg" x="0px" y="0px"
     width="140.171px" height="140.171px" viewBox="0 0 140.171 140.171" style="enable-background:new 0 0 140.171 140.171;"
     xml:space="preserve">
<g>
  <path d="M31.987,90.618c-11.048-5.12-17.97-12.437-17.97-20.564c0-13.055,17.882-23.996,42.051-27.11V28.848
    C24.078,32.746,0,49.713,0,70.054c0,11.729,8.021,22.303,20.925,29.92C35.043,108.604,42.051,94.588,31.987,90.618z
     M116.155,38.416c-7.521-6.906-18.035,3.607-10.865,9.822c12.724,5.14,20.865,13.004,20.865,21.816
    c0,13.062-17.885,24.004-42.052,27.117v14.09c31.99-3.892,56.068-20.858,56.068-41.207
    C140.171,57.423,130.853,46.116,116.155,38.416z M62.261,52.461L85.075,38.8c3.319-1.988,3.326-5.226,0.018-7.228L62.068,17.637
    c-3.312-1.999-5.985-0.489-5.969,3.381l0.124,28.034C56.233,52.923,58.938,54.449,62.261,52.461z M78.104,87.723L55.08,101.657
    c-3.311,2.002-3.304,5.24,0.019,7.229l22.811,13.661c3.319,1.984,6.03,0.462,6.047-3.412l0.12-28.034
    C84.089,87.233,81.413,85.725,78.104,87.723z"/>
</g>
</svg>
)=====";

const char deprecatedSvg[] = R"=====(
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 100 100">
  <path fill="currentColor" d="M38.75 78.13V36.88A1.85 1.85 0 0036.88 35h-3.75a1.85 1.85 0 00-1.88 1.88v41.25A1.85 1.85 0 0033.13 80h3.75a1.85 1.85 0
  001.87-1.87zm15 0V36.88A1.85 1.85 0 0051.88 35h-3.75a1.85 1.85 0 00-1.88 1.88v41.25A1.85 1.85 0 0048.13 80h3.75a1.85 1.85 0 001.87-1.87zm15
  0V36.88A1.85 1.85 0 0066.88 35h-3.75a1.85 1.85 0 00-1.88 1.88v41.25A1.85 1.85 0 0063.13 80h3.75a1.85 1.85 0 001.87-1.87zM36.88
  20h26.25l-2.82-6.85a2.35 2.35 0 00-1-.65H40.74a2 2 0 00-1 .65zm54.37 1.88v3.75a1.85 1.85 0 01-1.87 1.87h-5.63v55.55c0 6.44-4.22 12-9.37 12H25.63c-5.16
  0-9.38-5.27-9.38-11.72V27.5h-5.62a1.85 1.85 0 01-1.88-1.83v-3.8A1.85 1.85 0 0110.63 20h18.1l4.1-9.78A9.12 9.12 0 0140.63 5h18.75a9.1 9.1 0 017.79
  5.22l4.1 9.78h18.11a1.85 1.85 0 011.87 1.87z"></path>
</svg>
)=====";

void webGetTwoCirclingArrowsSvg(WiFiClient& client) {
  client.print(R"=====(
HTTP/1.0 200 OK
Content-type: image/svg+xml
Content-Length: )=====");
  client.println(strlen(twoCirclingArrowsSvg));
  client.println();
  client.print(twoCirclingArrowsSvg);
}

void webGetDeprecatedSvg(WiFiClient& client) {
  client.print(R"=====(
HTTP/1.0 200 OK
Content-type: image/svg+xml
Content-Length: )=====");
  client.println(strlen(deprecatedSvg));
  client.println();
  client.print(deprecatedSvg);
}
