/*  draw text's APP
    drawChar(INT8U ascii,INT16U poX, INT16U poY,INT16U size, INT16U fgcolor);
    drawString(char *string,INT16U poX, INT16U poY,INT16U size,INT16U fgcolor);
*/

#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>

void testColor() {
  for (int r = 0; r < 256; r ++) {
    for (int g = 0; g < 256; g ++) {
      for (int b = 0; b < 256; b ++) {
        Tft.setPixel(11 + g, 31 + b / 2, COLOR(r,g,b));
      }
    }
    Tft.drawVerticalLine(11 + r, 160, 70, COLOR(r,r,r));
  }
}

void setup()
{
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  // init TFT library
    
    Tft.drawChar('S',0,0,1,RED);                // draw char: 'S', (0, 0), size: 1, color: RED
    
    Tft.drawChar('E',10,10,2,BLUE);             // draw char: 'E', (10, 10), size: 2, color: BLUE
    
    Tft.drawChar('E',20,40,3,GREEN);            // draw char: 'E', (20, 40), size: 3, color: GREEN
    
    Tft.drawChar('E',30,80,4,YELLOW);           // draw char: 'E', (30, 80), size: 4, color: YELLOW
    
    Tft.drawChar('D',40,120,4,YELLOW);          // draw char: 'D', (40, 120), size: 4, color: YELLOW
    
    Tft.drawString("Hello",0,180,3,CYAN);       // draw string: "hello", (0, 180), size: 3, color: CYAN
    
    Tft.drawString("World!!",60,220,4,WHITE);    // draw string: "world!!", (80, 230), size: 4, color: WHITE
    
    Tft.drawRectangle(10, 30, 300, 201, WHITE);
    testColor();

}

void loop()
{
  
}
