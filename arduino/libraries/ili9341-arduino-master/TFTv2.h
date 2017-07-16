/*
 2012 Copyright (c) Seeed Technology Inc.

 Authors: Albert.Miao & Loovee, 
 Visweswara R (with initializtion code from TFT vendor)

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
#ifndef TFTv2_h
#define TFTv2_h

#if defined(ARDUINO) && ARDUINO >= 100
#define SEEEDUINO
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <avr/pgmspace.h>
#include "RussFontANSI.c"
#include <SPI.h>

//#define HAS_SERIAL
//#define HAS_SERIAL_DEBUG
#define FONT_SPACE 6
#define FONT_X 5
#define FONT_Y 8

#define COLOR(red, green, blue) \
  ((((red) << 8) & 0xf800) | (((green) << 3) & 0x07e0) | (((blue) >> 3) & 0x001f))

//Basic Colors
#define RED	0xf800
#define GREEN	0x07e0
#define BLUE	0x001f
#define BLACK	0x0000
#define YELLOW	0xffe0
#define WHITE	0xffff

//Other Colors
#define CYAN		0x07ff	
#define BRIGHT_RED	0xf810	
#define GRAY1		0x8410  
#define GRAY2		0x4208  

//TFT resolution 240*320
#define MIN_X	0
#define MIN_Y	0
#define MAX_X	239
#define MAX_Y	319

// ROTATION may be
// ROTATION_UP, ROTATION_RIGHT, ROTATION_LEFT or ROTATION_DOWN
//#define ROTATION_UP
//#define ROTATION_RIGHT
#define ROTATION_LEFT
//#define ROTATION_DOWN

#if defined(ROTATION_LEFT) || defined(ROTATION_RIGHT)
  #define MAX_POSITION_X	MAX_Y
#else /* ROTATION_UP || ROTATION_DOWN */
  #define MAX_POSITION_X	MAX_X
#endif

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#define TFT_CS_LOW  {DDRE |= 0x08;PORTE &=~ 0x08;}
#define TFT_CS_HIGH {DDRE |= 0x08;PORTE |=  0x08;}
#define TFT_DC_LOW  {DDRH |= 0x08;PORTH &=~ 0x08;}
#define TFT_DC_HIGH {DDRH |= 0x08;PORTH |=  0x08;}
#define TFT_BL_OFF  {DDRH |= 0x10;PORTH &=~ 0x10;}
#define TFT_BL_ON   {DDRH |= 0x10;PORTH |=  0x10;}
#define TFT_RST_OFF {DDRD |= 0x10;PORTD |=  0x10;}
#define TFT_RST_ON  {DDRD |= 0x10;PORTD &=~ 0x10;}

#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 54   // can be a digital pin, this is A0
#define XP 57   // can be a digital pin, this is A3

#elif defined(__AVR_ATmega32U4__)

#define TFT_CS_LOW  {DDRC |= 0x40;PORTC &=~ 0x40;}
#define TFT_CS_HIGH {DDRC |= 0x40;PORTC |=  0x40;}
#define TFT_DC_LOW  {DDRD |= 0x80;PORTD &=~ 0x80;}
#define TFT_DC_HIGH {DDRD |= 0x80;PORTD |=  0x80;}
#define TFT_BL_OFF  {DDRE |= 0x40;PORTE &=~ 0x40;}
#define TFT_BL_ON   {DDRE |= 0x40;PORTE |=  0x40;}
#define TFT_RST_OFF {DDRD |= 0x10;PORTD |=  0x10;}
#define TFT_RST_ON  {DDRD |= 0x10;PORTD &=~ 0x10;}

#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 18   // can be a digital pin, this is A0
#define XP 21   // can be a digital pin, this is A3

#else
#define TFT_CS_LOW  {DDRD |= 0x20;PORTD &=~ 0x20;}
#define TFT_CS_HIGH {DDRD |= 0x20;PORTD |=  0x20;}
#define TFT_DC_LOW  {DDRD |= 0x40;PORTD &=~ 0x40;}
#define TFT_DC_HIGH {DDRD |= 0x40;PORTD |=  0x40;}
#define TFT_BL_OFF  {DDRD |= 0x80;PORTD &=~ 0x80;}
#define TFT_BL_ON   {DDRD |= 0x80;PORTD |=  0x80;}
#define TFT_RST_OFF {DDRD |= 0x10;PORTD |=  0x10;}
#define TFT_RST_ON  {DDRD |= 0x10;PORTD &=~ 0x10;}


#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 14   // can be a digital pin, this is A0
#define XP 17   // can be a digital pin, this is A3

#endif

#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2

#ifndef INT8U
#define INT8U unsigned char
#endif
#ifndef INT16U
#define INT16U unsigned int
#endif
#ifndef INT32U
#define INT32U unsigned long
#endif

//extern INT8U simpleFont[][8];

class TFT {
public:
	void TFTinit (void);
    INT32U setInterval(
      int StartCol, int StartPage, int EndCol, int EndPage
    );
    void setPixel(INT16U poX, INT16U poY, INT16U color);
	void WRITE_Package(INT16U *data,INT8U howmany);
	INT8U Read_Register(INT8U Addr,INT8U xParameter);
	void fillScreen(INT16U XL,INT16U XR,INT16U YU,INT16U YD,INT16U color);
	void fillScreen(void);
	INT8U readID(void);

	void drawChar(INT8U ascii,INT16U poX, INT16U poY,INT16U size, INT16U fgcolor);
	void drawString(char *string,INT16U poX, INT16U poY,INT16U size,INT16U fgcolor);
	void fillRectangle(INT16U poX, INT16U poY, INT16U length, INT16U width, INT16U color);

	void drawLine(INT16U x0,INT16U y0,INT16U x1,INT16U y1,INT16U color);
	void drawVerticalLine(INT16U poX, INT16U poY,INT16U length,INT16U color);
	void drawHorizontalLine(INT16U poX, INT16U poY,INT16U length,INT16U color);
	void drawRectangle(INT16U poX, INT16U poY, INT16U length,INT16U width,INT16U color);

	void drawCircle(int poX, int poY, int r,INT16U color);
	void fillCircle(int poX, int poY, int r,INT16U color);

	void drawTraingle(int poX1, int poY1, int poX2, int poY2, int poX3, int poY3, INT16U color);
	INT8U drawNumber(long long_num,INT16U poX, INT16U poY,INT16U size,INT16U fgcolor);
	INT8U drawFloat(float floatNumber,INT8U decimal,INT16U poX, INT16U poY,INT16U size,INT16U fgcolor);
	INT8U drawFloat(float floatNumber,INT16U poX, INT16U poY,INT16U size,INT16U fgcolor);

private:
    void setCol(INT16U StartCol,INT16U EndCol);
    void setPage(INT16U StartPage,INT16U EndPage);
    void sendCMD(INT8U index);
    void WRITE_DATA(INT8U data);
    void sendData(INT16U data);
};

extern TFT Tft;

#endif

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
