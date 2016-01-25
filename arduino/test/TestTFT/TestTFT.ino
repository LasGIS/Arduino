#include <TFT_lg.h>
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
  TFTscreen.background(0, 0, 0);

  drawBox();
}

void loop() {
  delay(10);
}

void drawBox() {
  int xMin = 3;
  int yMin = 2;
  int xMax = TFTscreen.height();
  int yMax = TFTscreen.width()-2;
  Serial.print("x0 = ");
  Serial.print(xMin);
  Serial.print("; y0 = ");
  Serial.print(yMin);
  Serial.print("; xm = ");
  Serial.print(xMax);
  Serial.print("; ym = ");
  Serial.print(yMax);
  Serial.print("; xlen(width) = ");
  Serial.print(xMax - xMin);
  Serial.print("; ylen(height) = ");
  Serial.print(yMax - yMin);
  Serial.println(";");
  
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.line(1, 15, 15, 1);

  // draw a line in a nice color
  TFTscreen.stroke(250, 0, 0);
  TFTscreen.line(xMin, yMin, xMax, yMin);
  TFTscreen.line(xMax, yMin, xMax, yMax);
  TFTscreen.line(xMax, yMax, xMin, yMax);
  TFTscreen.line(xMin, yMax, xMin, yMin);

  TFTscreen.stroke(250, 180, 10);
  TFTscreen.line(xMin, yMin, xMax, yMax);
  TFTscreen.line(xMin, yMax, xMax, yMin);
}
