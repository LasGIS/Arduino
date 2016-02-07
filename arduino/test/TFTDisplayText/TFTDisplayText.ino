/*
  Arduino TFT text example

  This example demonstrates how to draw text on the
  TFT with an Arduino. The Arduino reads the value
  of an analog sensor attached to pin A0, and writes
  the value to the LCD screen, updating every
  quarter second.

  This example code is in the public domain

  Created 15 April 2013 by Scott Fitzgerald

  http://www.arduino.cc/en/Tutorial/TFTDisplayText

 */

#include <TFT_lg.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8

// create an instance of the library
TFT screen = TFT(cs, dc, rst);

// char array to print to the screen
char sensorPrintout[5];

void setup() {
  // begin serial communication
  Serial.begin(9600);

  // Put this line at the beginning of every sketch that uses the GLCD:
  screen.begin();

  // clear the screen with a black background
  screen.background(0, 0, 0);

  // write the static text to the screen
  // set the font color to white
  screen.stroke(255, 255, 255);
  // set the font size
  screen.setTextSize(1);
  showChars();
/*
  // write the text to the top left corner of the screen
  screen.text("Sensor Value:\n абвгдеёжз", 0, 0);
  // ste the font size very large for the loop
  screen.setTextSize(4);
*/
}

void showChars() {
  screen.setCursor(0, 0);
  for (uint16_t i = 0; i < 16; i++) {
    for (uint16_t j = 0; j < 16; j++) {
      uint16_t chr = i * 16 + j;
      if (chr == '\n' || chr == '\r') {
        screen.write('?');
      } else {
        screen.write(chr);
      }
    }
    screen.println(String("|") + i);
  }
}

void loop() {
/*
  // Read the value of the sensor on A0
  int sensor = analogRead(A0);
  Serial.println(sensor);
  String sensorVal = String(sensor);

  // convert the reading to a char array
  sensorVal.toCharArray(sensorPrintout, 5);

  // set the font color
  screen.stroke(255, 0, 255);
  // print the sensor value
  screen.text(sensorPrintout, 0, 20);
  // wait for a moment
  delay(250);
  // erase the text you just wrote
  screen.stroke(0, 0, 0);
  screen.text(sensorPrintout, 0, 20);
*/
}

