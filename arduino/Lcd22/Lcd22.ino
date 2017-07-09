#include <Arduino.h>
#include <avr/pgmspace.h>
#include <TFTv2.h>
#include <SPI.h>
#include "Lcd22.h"

#define MEASURIES_SIZE 6
#define FONT_SIZE 1
#define CHAR_WIDTH FONT_SPACE * FONT_SIZE
#define CHAR_HEIGHT FONT_Y * FONT_SIZE

LgMeasure::LgMeasure(const char* _description,
    uint8_t _pin, uint8_t _decimal,
    uint16_t _color,
    float _factor
) {
    char* buffer = new char[strlen_P(_description) + 2];
    strcpy_P(buffer, _description);
    description = buffer;
    pin = _pin;
    decimal = _decimal;
    color = _color;
    factor = _factor;
}

const char prgm_str00[] PROGMEM = "нап Зарядка = ";
const char prgm_str01[] PROGMEM = "ток Батареи = ";
const char prgm_str02[] PROGMEM = "нап Батарея = ";
const char prgm_str03[] PROGMEM = "ток  +5     = ";
const char prgm_str04[] PROGMEM = "напр +5     = ";
const char prgm_str05[] PROGMEM = "напр +3.3   = ";

const LgMeasure* measuries[] = {
  /* нап Зарядка */
  new LgMeasure(prgm_str00, A0, 2, CHARGER_VT_COLOR, 0.01175),
  /* нап Батарея                   __RRRRRggggggBBBBB */
  new LgMeasure(prgm_str02, A2, 2, BATTERY_VT_COLOR, 0.00630),
  /* напр +5                       __RRRRRggggggBBBBB */
  new LgMeasure(prgm_str04, A6, 2, SUPPLY5_VT_COLOR, 0.01175),

  /* напр +3.3                      __RRRRRggggggBBBBB */
  new LgMeasure(prgm_str05, A7, 2, SUPPLY3_VT_COLOR, 0.00630),
  /* ток Батареи                   __RRRRRggggggBBBBB */
  new LgMeasure(prgm_str01, A1, 3, BATTERY_IT_COLOR, 0.00106818),
  /* ток  +5                       __RRRRRggggggBBBBB */
  new LgMeasure(prgm_str03, A3, 3, SUPPLY5_IT_COLOR, 0.0007477),
};

void setup() {
    analogReference(INTERNAL);
    Serial.begin(9600);
    TFT_BL_ON;                                  // turn on the background light
    Tft.TFTinit();                              //init TFT library
    Tft.drawRectangle(10, 30, 300, 200, RED);
//    Tft.drawLine(0,0,239,319,RED);
//    Tft.drawVerticalLine(60,100,100,GREEN);
//    Tft.drawHorizontalLine(30,60,150,BLUE);
    drawFirst();
}

void drawFirst(){
  for (int i = 0; i < MEASURIES_SIZE; i++) {
    int col = i / 3;
    int pos = i % 3;
    Tft.drawString(
      measuries[i]->description,
      CHAR_WIDTH * col * 20, CHAR_HEIGHT * pos, FONT_SIZE,
      WHITE
    );
  }
}

void loop() {
  for (int i = 0; i < MEASURIES_SIZE; i++) {
    int col = i / 3;
    int pos = i % 3;
    float val = analogRead(measuries[i]->pin) * measuries[i]->factor;
#ifdef HAS_SERIAL
    Serial.print(measuries[i]->description);
    Serial.println(val, 3);
#endif
    Tft.fillRectangle(
      CHAR_WIDTH * (col * 20 + 14), CHAR_HEIGHT * pos,
      CHAR_WIDTH * 6              , CHAR_HEIGHT, BLACK
    );
    Tft.drawFloat(
      val, measuries[i]->decimal,
      CHAR_WIDTH * (col * 20 + 14), CHAR_HEIGHT * pos, FONT_SIZE,
      measuries[i]->color
    );
  }
#ifdef HAS_SERIAL
  Serial.println(" ----------- ");
#endif
  delay(500);
}

