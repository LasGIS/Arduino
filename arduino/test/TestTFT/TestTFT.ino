#include <TFT.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

TFT TFTscreen = TFT(cs, dc, rst);

void setup() {
  // begin serial communication
  Serial.begin(9600);

  // initialize the display
  TFTscreen.begin();

  // set the background to white
  TFTscreen.background(255, 255, 255);

}

void loop() {
  static int xMin = 0;
  static int yMin = 0;
  static int xMax = TFTscreen.height() - 1;
  static int yMax = TFTscreen.width() - 1;

  // draw a line in a nice color
  TFTscreen.stroke(250, 180, 10);
  TFTscreen.line(xMin, yMin, xMax, yMax);
  TFTscreen.line(xMin, yMax, xMax, yMin);

  delay(160);
}
