#include <Arduino.h>
#include <avr/pgmspace.h>
#include <TFTv2.h>
#include <SPI.h>
#include "Lcd22.h"

LgMeasure::LgMeasure(
    const char* _description,
    uint8_t _pin,
    uint16_t _color,
    float _factor
) {
    char* buffer = new char[strlen_P(_description) + 2];
    strcpy_P(buffer, _description);
    description = buffer;
    pin = _pin;
    color = _color;
    factor = _factor;
}

const char prgm_str00[] PROGMEM = "Зарядка     = ";
const char prgm_str01[] PROGMEM = "ток Батареи = ";
const char prgm_str02[] PROGMEM = "Батарея     = ";
const char prgm_str03[] PROGMEM = "ток +5      = ";
const char prgm_str04[] PROGMEM = "напр +5     = ";
const char prgm_str05[] PROGMEM = "напр +3.3   = ";

const LgMeasure* measuries[6] = {
    new LgMeasure(prgm_str00, A0, WHITE, 0.01175),
    new LgMeasure(prgm_str01, A1, WHITE, 1.0),
    new LgMeasure(prgm_str02, A2, WHITE, 0.00630),
    new LgMeasure(prgm_str03, A3, WHITE, 1.0),
    new LgMeasure(prgm_str04, A6, WHITE, 0.01175),
    new LgMeasure(prgm_str05, A7, WHITE, 0.00630),
};

void setup() {
    analogReference(INTERNAL);
    Serial.begin(9600);
    TFT_BL_ON;                                  // turn on the background light
    Tft.TFTinit();                              //init TFT library
    Tft.drawRectangle(0,0,239,319,WHITE);
    Tft.drawLine(0,0,239,319,RED);
    Tft.drawVerticalLine(60,100,100,GREEN);     // Draw a vertical line
                                                // start: (60, 100) length: 100 color: green
    Tft.drawHorizontalLine(30,60,150,BLUE);     //Draw a horizontal line
    Tft.drawString("String", 2, 2, 2, WHITE);
}

void loop() {
    for (int i = 0; i < 6/*sizeof(measuries)*/; i++) {
        Serial.print(measuries[i]->description);
        float val = analogRead(measuries[i]->pin) * measuries[i]->factor;
        Serial.println(val,3);
    }
    Serial.println(" ----------- ");
    delay(2000);
}

