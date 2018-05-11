// Include application, user and local libraries
#include "SPI.h"
#include "TFT_LG_ILI9225.h"

/*
#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
#define TFT_LED 3   // 0 if wired to +5V directly
*/
TFT_LG_ILI9225 tft;

// Setup
void setup() {
  tft.begin();
  Serial.begin(9600);
}

// Loop
void loop() {
}
