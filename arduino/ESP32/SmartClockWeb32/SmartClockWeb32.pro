TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/cores/esp32/IPAddress.cpp \
    C:/Arduino/arduino/libraries/Json/src/Json.cpp \
    C:/Arduino/arduino/libraries/DS3231/DS3231.cpp \
    C:/Arduino/arduino/libraries/TFT_eSPI/TFT_eSPI.cpp \
    SmartClockWeb32.ino \
    utility.cpp \
    IndexHtml.cpp \

# Specifies include paths that are passed to the Windows Resource Compiler.
INCLUDEPATH += \
    C:/Arduino/arduino/libraries/Json/src \
    C:/Arduino/arduino/libraries/DS3231 \
    C:/Arduino/arduino/libraries/TFT_eSPI \
    C:/Arduino/arduino/libraries/TFT_eSPI/Fonts \
    C:/Arduino/arduino/libraries/TFT_eSPI/Fonts/Custom \
    C:/Arduino/arduino/libraries/TFT_eSPI/Fonts/GFXFF \
    C:/Arduino/arduino/libraries/TFT_eSPI/Fonts/TrueType \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/cores/esp32 \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/SPI/src \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/WiFi/src \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/EEPROM/src \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/ArduinoOTA/src \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/SoftwareSerial/src \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/SPI/src \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/Wire/src \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/libraries/Wire/src/utility \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/variants/esp32 \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/tools/sdk/esp32/include/freertos/include \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/tools/sdk/esp32/include/soc/esp32/include/soc \

HEADERS += \
    C:/Arduino15/packages/esp32/hardware/esp32/2.0.11/variants/esp32/pins_arduino.h \
    C:/Arduino/arduino/libraries/Json/src/Json.h \
    C:/Arduino/arduino/libraries/DS3231/DS3231.h \
    C:/Arduino/arduino/libraries/TFT_eSPI/User_Setup.h \
    C:/Arduino/arduino/libraries/TFT_eSPI/User_Setup_Select.h \
    C:/Arduino/arduino/libraries/TFT_eSPI/TFT_config.h \
    SmartClockWeb32.h \
