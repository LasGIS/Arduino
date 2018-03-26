#-------------------------------------------------
#
# Project created by QtCreator 2017-02-09T13:21:05
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lcd22
TEMPLATE = app

#// THE_ANSWER 42
DEFINES += ARDUINO=10612
DEFINES += F_CPU 16000000L
DEFINES += ARDUINO_AVR_PRO
DEFINES += ARDUINO_ARCH_AVR
DEFINES += __AVR__
DEFINES += __AVR_ATmega328P__

# Specifies include paths that are passed to the Windows Resource Compiler.
INCLUDEPATH += ../libraries/arduino-ds1302-master \
  ../libraries/DHT-sensor-library-master \
  ../libraries/ili9341-arduino-master \
  ../libraries/IrControl \
  ../libraries/LiquidCrystal_I2C \
  ../libraries/MsTimer2 \
  ../libraries/DS3231 \
  C:/local/Arduino/hardware/arduino/avr/cores/arduino \
  C:/local/Arduino/hardware/tools/avr/avr/include \
  C:/local/Arduino/hardware/arduino/avr/variants/standard \
  C:/local/Arduino/hardware/arduino/avr/libraries/EEPROM/src \
  C:/local/Arduino/hardware/arduino/avr/libraries/HID/src \
  C:/local/Arduino/hardware/arduino/avr/libraries/SoftwareSerial/src \
  C:/local/Arduino/hardware/arduino/avr/libraries/SPI/src \
  C:/local/Arduino/hardware/arduino/avr/libraries/Wire/src \
  C:/local/Arduino/hardware/arduino/avr/libraries/Wire/src/utility \
  C:/local/Arduino/libraries/SD/src \
  C:/local/Arduino/libraries/Servo/src


SOURCES += \
    ../libraries/arduino-ds1302-master/DS1302.cpp \
    ../libraries/DHT-sensor-library-master/DHT.cpp \
    ../libraries/IrControl/IrControl.cpp \
    ../libraries/MsTimer2/MsTimer2.cpp \
    ../libraries/ili9341-arduino-master/font.c \
    ../libraries/ili9341-arduino-master/TFTv2.cpp \
    Lcd22.ino \
    ../libraries/ili9341-arduino-master/RussFontANSI.c \
    ../libraries/ili9341-arduino-master/RussFontOEM.c \
    ../libraries/ADXL345/src/ADXL345.cpp \
    ../libraries/AT24C32/EEPROM.cpp \
    ../libraries/DS3231/DS3231.cpp \
    ../libraries/iarduino_RTC/src/iarduino_RTC.cpp \
    ../libraries/SD/src/utility/Sd2Card.cpp \
    ../libraries/SD/src/utility/SdFile.cpp \
    ../libraries/SD/src/utility/SdVolume.cpp \
    ../libraries/SD/src/File.cpp \
    ../libraries/SD/src/SD.cpp \
    RealTime.cpp \
    ../libraries/Adafruit_ADXL345-master/Adafruit_ADXL345_U.cpp

HEADERS += \
    ../libraries/arduino-ds1302-master/DS1302.h \
    ../libraries/DHT-sensor-library-master/DHT.h \
    ../libraries/IrControl/IrControl.h \
    ../libraries/MsTimer2/MsTimer2.h \
    ../libraries/ili9341-arduino-master/TFTv2.h \
    Lcd22.h \
    ../libraries/ADXL345/src/ADXL345.h \
    ../libraries/AT24C32/EEPROM.h \
    ../libraries/DS3231/DS3231.h \
    ../libraries/iarduino_RTC/src/iarduino_RTC.h \
    ../libraries/iarduino_RTC/src/iarduino_RTC_DS1302.h \
    ../libraries/iarduino_RTC/src/iarduino_RTC_DS1307.h \
    ../libraries/iarduino_RTC/src/iarduino_RTC_DS3231.h \
    ../libraries/iarduino_RTC/src/memorysaver.h \
    ../libraries/SD/examples/CardInfo/CardInfo.ino \
    ../libraries/SD/examples/Datalogger/Datalogger.ino \
    ../libraries/SD/examples/DumpFile/DumpFile.ino \
    ../libraries/SD/examples/Files/Files.ino \
    ../libraries/SD/examples/listfiles/listfiles.ino \
    ../libraries/SD/examples/ReadWrite/ReadWrite.ino \
    ../libraries/SD/src/utility/FatStructs.h \
    ../libraries/SD/src/utility/Sd2Card.h \
    ../libraries/SD/src/utility/Sd2PinMap.h \
    ../libraries/SD/src/utility/SdFat.h \
    ../libraries/SD/src/utility/SdFatmainpage.h \
    ../libraries/SD/src/utility/SdFatUtil.h \
    ../libraries/SD/src/utility/SdInfo.h \
    ../libraries/SD/src/SD.h \
    ../libraries/Adafruit_ADXL345-master/Adafruit_ADXL345_U.h \
    ../libraries/Adafruit_Sensor-master/Adafruit_Sensor.h

#FORMS    += mainwindow.ui
DISTFILES += \
    ../libraries/ili9341-arduino-master/README.md \
    ../libraries/ili9341-arduino-master/keywords.txt \
    ../libraries/SD/src/README.txt \
    ../libraries/SD/keywords.txt \
    ../libraries/SD/library.properties \
    ../libraries/SD/README.adoc
