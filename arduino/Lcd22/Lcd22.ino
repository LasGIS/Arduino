#include <Arduino.h>
#include <avr/pgmspace.h>
#include <TFTv2.h>
#include <SPI.h>
#include "Lcd22.h"

#define MEASURIES_SIZE 6
#define FONT_SIZE 2

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
const char prgm_str03[] PROGMEM = "ток  +5     = ";
const char prgm_str04[] PROGMEM = "напр +5     = ";
const char prgm_str05[] PROGMEM = "напр +3.3   = ";

const LgMeasure* measuries[] = {
    new LgMeasure(prgm_str00, A0, BLUE,  0.01175),
    new LgMeasure(prgm_str01, A1, RED,   1.0),
    new LgMeasure(prgm_str02, A2, BLUE,  0.00630),
    new LgMeasure(prgm_str03, A3, RED,   1.0),
    new LgMeasure(prgm_str04, A6, BLUE,  0.01175),
    new LgMeasure(prgm_str05, A7, BLUE,  0.00630),
};

void setup() {
    analogReference(INTERNAL);
    Serial.begin(9600);
    TFT_BL_ON;                                  // turn on the background light
    Tft.TFTinit();                              //init TFT library
//    Tft.drawRectangle(0,0,239,319,RED);
//    Tft.drawLine(0,0,239,319,RED);
//    Tft.drawVerticalLine(60,100,100,GREEN);
//    Tft.drawHorizontalLine(30,60,150,BLUE);
    drawFirst();
    //Tft.drawString("String", 2, 2, 2, WHITE);
}

void drawFirst(){
  for (int i = 0; i < MEASURIES_SIZE; i++) {
    Tft.drawString(
      measuries[i]->description,
      0, 0 + (FONT_Y + 1) * FONT_SIZE * i, FONT_SIZE,
      WHITE
    );
  }
}

void loop() {
  for (int i = 0; i < MEASURIES_SIZE; i++) {
    float val = analogRead(measuries[i]->pin) * measuries[i]->factor;
#ifdef HAS_SERIAL
    Serial.print(measuries[i]->description);
    Serial.println(val, 3);
#endif
    Tft.fillRectangle(
      FONT_SPACE * FONT_SIZE * 14, 0 + (FONT_Y + 1) * FONT_SIZE * i,
      FONT_SPACE * FONT_SIZE * 6, FONT_Y * FONT_SIZE, BLACK
    );
    Tft.drawFloat(
      val, FONT_SPACE * FONT_SIZE * 14, 0 + (FONT_Y + 1) * FONT_SIZE * i, FONT_SIZE,
      measuries[i]->color
    );
  }
#ifdef HAS_SERIAL
  Serial.println(" ----------- ");
#endif
  delay(2000);
}

