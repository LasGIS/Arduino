TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    C:/Arduino15/packages/esp8266/hardware/esp8266/3.1.2/cores/esp8266/IPAddress.cpp \
    C:/Arduino/arduino/libraries/DS3231/DS3231.cpp \
    C:/Arduino/arduino/libraries/TFT_eSPI/TFT_eSPI.cpp \
    C:\Arduino\arduino\libraries\Arduino_JSON\src\cjson\cJSON.c \
    SmartClockWeb8266.ino \
    RealTime.cpp \
    utility.cpp \
    webApiControllers.cpp \
    webImages.cpp \
    webIndexHtml.cpp \
    webMain.cpp \
    webSrcJs.cpp \
    webStatic.cpp \

# Specifies include paths that are passed to the Windows Resource Compiler.
INCLUDEPATH += \
    C:/Arduino/arduino/libraries/DS3231 \
    C:\Arduino\arduino\libraries\Arduino_JSON\src \
    C:/Arduino/arduino/libraries/TFT_eSPI \
    C:/Arduino/arduino/libraries/TFT_eSPI/Fonts \
    C:/Arduino/arduino/libraries/TFT_eSPI/Fonts/Custom \
    C:/Arduino/arduino/libraries/TFT_eSPI/Fonts/GFXFF \
    C:/Arduino/arduino/libraries/TFT_eSPI/Fonts/TrueType \
    C:/Arduino15/packages/esp8266/hardware/esp8266/3.1.2/cores/esp8266 \
    C:/Arduino15/packages/esp8266/hardware/esp8266/3.1.2/libraries/ESP8266WiFi/src \
    C:/Arduino15/packages/esp8266/hardware/esp8266/3.1.2/libraries/ESP8266WiFi/src/include \
    C:\Arduino15\packages\esp8266\hardware\esp8266\3.1.2\libraries\ESP8266WebServer\src \
    C:/Arduino15/packages/esp8266/hardware/esp8266/3.1.2/libraries/EEPROM \
    C:/Arduino15/packages/esp8266/hardware/esp8266/3.1.2/libraries/ArduinoOTA \
    C:/Arduino15/packages/esp8266/hardware/esp8266/3.1.2/libraries/SoftwareSerial/src \
    C:/Arduino15/packages/esp8266/hardware/esp8266/3.1.2/libraries/SoftwareSerial/src/circular_queue \
    C:/Arduino15/packages/esp8266/hardware/esp8266/3.1.2/libraries/SPI \
    C:/Arduino15/packages/esp8266/hardware/esp8266/3.1.2/libraries/Wire \
    C:\Arduino15\packages\esp8266\hardware\esp8266\3.1.2\variants\generic \

HEADERS += \
    C:\Arduino15\packages\esp8266\hardware\esp8266\3.1.2\variants\generic\common.h \
    C:/Arduino15/packages/esp8266/hardware/esp8266/3.1.2/variants/generic/pins_arduino.h \
    C:\Arduino15\packages\esp8266\hardware\esp8266\3.1.2\libraries\ESP8266WebServer\src\Parsing-impl.h \
    C:/Arduino/arduino/libraries/DS3231/DS3231.h \
    C:/Arduino/arduino/libraries/TFT_eSPI/User_Setup.h \
    C:/Arduino/arduino/libraries/TFT_eSPI/User_Setup_Select.h \
    C:/Arduino/arduino/libraries/TFT_eSPI/TFT_config.h \
    SmartClockWeb8266.h \
