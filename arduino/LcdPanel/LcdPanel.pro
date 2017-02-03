TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    ../libraries/arduino-ds1302-master/DS1302.cpp \
    ../libraries/DHT-sensor-library-master/DHT.cpp \
    ../libraries/IrControl/IrControl.cpp \
    ../libraries/LiquidCrystal_I2C/LiquidCrystal_I2C.cpp \
    ../libraries/MsTimer2/MsTimer2.cpp \
    ../libraries/ili9341-arduino-master/font.c \
    alarm_clock.cpp \
    buzzer.cpp \
    CustomChars.cpp \
    lcd_screen.cpp \
    main_screen.cpp \
    set_screen.cpp \
    show_char_screen.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Specifies include paths that are passed to the Windows Resource Compiler.
INCLUDEPATH += ../libraries/arduino-ds1302-master \
  ../libraries/DHT-sensor-library-master \
  ../libraries/ili9341-arduino-master \
  ../libraries/IrControl \
  ../libraries/LiquidCrystal_I2C \
  ../libraries/MsTimer2 \
  D:/local/Arduino/hardware/arduino/avr/cores/arduino \
  D:/local/Arduino/hardware/tools/avr/avr/include \
  D:/local/Arduino/hardware/arduino/avr/variants/standard \
  D:/local/Arduino/hardware/arduino/avr/libraries/EEPROM/src \
  D:/local/Arduino/hardware/arduino/avr/libraries/HID/src \
  D:/local/Arduino/hardware/arduino/avr/libraries/SoftwareSerial/src \
  D:/local/Arduino/hardware/arduino/avr/libraries/SPI/src \
  D:/local/Arduino/hardware/arduino/avr/libraries/Wire/src \
  D:/local/Arduino/hardware/arduino/avr/libraries/Wire/src/utility \
  D:/local/Arduino/libraries/SD/src \
  D:/local/Arduino/libraries/Servo/src

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../libraries/arduino-ds1302-master/DS1302.h \
    ../libraries/DHT-sensor-library-master/DHT.h \
    ../libraries/IrControl/IrControl.h \
    ../libraries/LiquidCrystal_I2C/LiquidCrystal_I2C.h \
    ../libraries/MsTimer2/MsTimer2.h \
    alarm_clock.h \
    lcd_screen.h \
    LcdPanel.h \
    LcdPanel.ino \
    main_screen.h \
    pitches.h \
    set_screen.h \
    show_char_screen.h

DISTFILES += \
    LcdPanel.fzz
