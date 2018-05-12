#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>

// Setup
void setup() {
//    TFT_BL_ON;
  Tft.TFTinit();
  Serial.begin(9600);
}

// Loop
void loop() {
}
