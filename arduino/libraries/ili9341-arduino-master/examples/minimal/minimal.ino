/*  Demo of draw line's APP
    drawLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int color);
    drawVerticalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
    drawHorizontalLine(unsigned int poX, unsigned int poY,unsigned int length,unsigned int color);
*/

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

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
