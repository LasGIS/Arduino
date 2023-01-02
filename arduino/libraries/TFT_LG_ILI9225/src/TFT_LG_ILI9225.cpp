#include "TFT_LG_ILI9225.h"
#include <avr/pgmspace.h>
#include <limits.h>
#include <SPI.h>

Size::Size(uint16_t _width, uint16_t _height) {
  width = _width;
  height = _height;
}
uint16_t Size::count() {
  return width * height;
}

void TFT_LG_ILI9225::_writecommand(uint8_t c) {
  TFT_DC_LOW;
  TFT_CS_LOW;
  SPI.transfer(c);
  TFT_CS_HIGH;
}

void TFT_LG_ILI9225::_writedata(uint8_t c) {
  TFT_DC_HIGH;
  TFT_CS_LOW;
  SPI.transfer(c);
  TFT_CS_HIGH;
}

void TFT_LG_ILI9225::_orientCoordinates(int16_t &x1, int16_t &y1) {
  switch (_orientation) {
  case 0:
    break;
  case 1:
    y1 = _maxY - y1 - 1;
    _swap(x1, y1);
    break;
  case 2:
    x1 = _maxX - x1 - 1;
    y1 = _maxY - y1 - 1;
    break;
  case 3:
    x1 = _maxX - x1 - 1;
    _swap(x1, y1);
    break;
  }
  x1 = constrain(x1, 0, ILI9225_LCD_WIDTH - 1);
  y1 = constrain(y1, 0, ILI9225_LCD_HEIGHT - 1);
}

Size TFT_LG_ILI9225::_setWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {

  _orientCoordinates(x0, y0);
  _orientCoordinates(x1, y1);

  if (x1 < x0) _swap(x0, x1);
  if (y1 < y0) _swap(y0, y1);

  _writeRegister(ILI9225_HORIZONTAL_WINDOW_ADDR1, x1);
  _writeRegister(ILI9225_HORIZONTAL_WINDOW_ADDR2, x0);

  _writeRegister(ILI9225_VERTICAL_WINDOW_ADDR1, y1);
  _writeRegister(ILI9225_VERTICAL_WINDOW_ADDR2, y0);

  _writeRegister(ILI9225_RAM_ADDR_SET1, x0);
  _writeRegister(ILI9225_RAM_ADDR_SET2, y0);

  _writecommand(0x00);
  _writecommand(0x22);

  return Size(x1 - x0 + 1, y1 - y0 + 1);
}


void TFT_LG_ILI9225::begin() {

  // Set up pins
#ifdef __LINKIT_ONE__
//  if (TFT_LED > 0) pinMode(TFT_LED, OUTPUT);

  pinMode(TFT_RST, OUTPUT);
  pinMode(TFT_RS, OUTPUT);
  pinMode(TFT_CS, OUTPUT);
#endif

  SPI.begin();

  // Turn on backlight
//  if (TFT_LED > 0) digitalWrite(TFT_LED, HIGH);

  // Initialization Code
  TFT_RST_ON;
  delay(10);
  TFT_RST_OFF;
  delay(50);

  /* Start Initial Sequence */

  /* Set SS bit and direction output from S528 to S1 */
  _writeRegister(ILI9225_POWER_CTRL1, 0x0000); // Set SAP,DSTB,STB
  _writeRegister(ILI9225_POWER_CTRL2, 0x0000); // Set APON,PON,AON,VCI1EN,VC
  _writeRegister(ILI9225_POWER_CTRL3, 0x0000); // Set BT,DC1,DC2,DC3
  _writeRegister(ILI9225_POWER_CTRL4, 0x0000); // Set GVDD
  _writeRegister(ILI9225_POWER_CTRL5, 0x0000); // Set VCOMH/VCOML voltage
  delay(40);

  // Power-on sequence
  _writeRegister(ILI9225_POWER_CTRL2, 0x0018); // Set APON,PON,AON,VCI1EN,VC
  _writeRegister(ILI9225_POWER_CTRL3, 0x6121); // Set BT,DC1,DC2,DC3
  _writeRegister(ILI9225_POWER_CTRL4, 0x006F); // Set GVDD   /*007F 0088 */
  _writeRegister(ILI9225_POWER_CTRL5, 0x495F); // Set VCOMH/VCOML voltage
  _writeRegister(ILI9225_POWER_CTRL1, 0x0800); // Set SAP,DSTB,STB
  delay(10);
  _writeRegister(ILI9225_POWER_CTRL2, 0x103B); // Set APON,PON,AON,VCI1EN,VC
  delay(50);

  _writeRegister(ILI9225_DRIVER_OUTPUT_CTRL, 0x011C); // set the display line number and display direction
  _writeRegister(ILI9225_LCD_AC_DRIVING_CTRL, 0x0100); // set 1 line inversion
  _writeRegister(ILI9225_ENTRY_MODE, 0x1030); // set GRAM write direction and BGR=1.
  _writeRegister(ILI9225_DISP_CTRL1, 0x0000); // Display off
  _writeRegister(ILI9225_BLANK_PERIOD_CTRL1, 0x0808); // set the back porch and front porch
  _writeRegister(ILI9225_FRAME_CYCLE_CTRL, 0x1100); // set the clocks number per line
  _writeRegister(ILI9225_INTERFACE_CTRL, 0x0000); // CPU interface
  _writeRegister(ILI9225_OSC_CTRL, 0x0D01); // Set Osc  /*0e01*/
  _writeRegister(ILI9225_VCI_RECYCLING, 0x0020); // Set VCI recycling
  _writeRegister(ILI9225_RAM_ADDR_SET1, 0x0000); // RAM Address
  _writeRegister(ILI9225_RAM_ADDR_SET2, 0x0000); // RAM Address

  /* Set GRAM area */
  _writeRegister(ILI9225_GATE_SCAN_CTRL, 0x0000);
  _writeRegister(ILI9225_VERTICAL_SCROLL_CTRL1, 0x00DB);
  _writeRegister(ILI9225_VERTICAL_SCROLL_CTRL2, 0x0000);
  _writeRegister(ILI9225_VERTICAL_SCROLL_CTRL3, 0x0000);
  _writeRegister(ILI9225_PARTIAL_DRIVING_POS1, 0x00DB);
  _writeRegister(ILI9225_PARTIAL_DRIVING_POS2, 0x0000);
  _writeRegister(ILI9225_HORIZONTAL_WINDOW_ADDR1, 0x00AF);
  _writeRegister(ILI9225_HORIZONTAL_WINDOW_ADDR2, 0x0000);
  _writeRegister(ILI9225_VERTICAL_WINDOW_ADDR1, 0x00DB);
  _writeRegister(ILI9225_VERTICAL_WINDOW_ADDR2, 0x0000);

  /* Set GAMMA curve */
  _writeRegister(ILI9225_GAMMA_CTRL1, 0x0000);
  _writeRegister(ILI9225_GAMMA_CTRL2, 0x0808);
  _writeRegister(ILI9225_GAMMA_CTRL3, 0x080A);
  _writeRegister(ILI9225_GAMMA_CTRL4, 0x000A);
  _writeRegister(ILI9225_GAMMA_CTRL5, 0x0A08);
  _writeRegister(ILI9225_GAMMA_CTRL6, 0x0808);
  _writeRegister(ILI9225_GAMMA_CTRL7, 0x0000);
  _writeRegister(ILI9225_GAMMA_CTRL8, 0x0A00);
  _writeRegister(ILI9225_GAMMA_CTRL9, 0x0710);
  _writeRegister(ILI9225_GAMMA_CTRL10, 0x0710);

  _writeRegister(ILI9225_DISP_CTRL1, 0x0012);
  delay(50);
  _writeRegister(ILI9225_DISP_CTRL1, 0x1017);

//  setBacklight(true);
  setOrientation(0);

  // Initialize variables
  setBackgroundColor(COLOR_BLACK );

  clear();
}


void TFT_LG_ILI9225::clear() {
  _fontSize = 1;
  uint8_t old = _orientation;
  setOrientation(0);
  fillRectangle(0, 0, _maxX - 1, _maxY - 1, COLOR_BLACK);
  setOrientation(old);
  delay(10);
}


void TFT_LG_ILI9225::invert(boolean flag) {
  _writecommand(0x00);
  _writecommand(flag ? ILI9225C_INVON : ILI9225C_INVOFF);
}

/*
void TFT_LG_ILI9225::setBacklight(boolean flag) {
  if (TFT_LED) digitalWrite(TFT_LED, flag);
}

void TFT_LG_ILI9225::setDisplay(boolean flag) {
  if (flag) {
    _writeRegister(0x00ff, 0x0000);
    _writeRegister(ILI9225_POWER_CTRL1, 0x0000);
    delay(50);
    _writeRegister(ILI9225_DISP_CTRL1, 0x1017);
    delay(200);
  } else {
    _writeRegister(0x00ff, 0x0000);
    _writeRegister(ILI9225_DISP_CTRL1, 0x0000);
    delay(50);
    _writeRegister(ILI9225_POWER_CTRL1, 0x0003);
    delay(200);
  }
}
*/

void TFT_LG_ILI9225::setOrientation(uint8_t orientation) {

  _orientation = orientation % 4;

  switch (_orientation) {
  case 0:
    _maxX = ILI9225_LCD_WIDTH;
    _maxY = ILI9225_LCD_HEIGHT;
    break;
  case 1:
    _maxX = ILI9225_LCD_HEIGHT;
    _maxY = ILI9225_LCD_WIDTH;
    break;
  case 2:
    _maxX = ILI9225_LCD_WIDTH;
    _maxY = ILI9225_LCD_HEIGHT;
    break;
  case 3:
    _maxX = ILI9225_LCD_HEIGHT;
    _maxY = ILI9225_LCD_WIDTH;
    break;
  }
}


uint8_t TFT_LG_ILI9225::getOrientation() {
  return _orientation;
}


void TFT_LG_ILI9225::drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
  fillRectangle(x1, y1, x1, y2, color);
  fillRectangle(x1, y1, x2, y1, color);
  fillRectangle(x1, y2, x2, y2, color);
  fillRectangle(x2, y1, x2, y2, color);
}


void TFT_LG_ILI9225::fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {

  uint16_t count = _setWindow(x1, y1, x2, y2).count();

  for(uint16_t t = count; t > 0; t--)
    _writeData(color);
}


void TFT_LG_ILI9225::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color) {

  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0, y0 + r, color);
  drawPixel(x0, y0 - r, color);
  drawPixel(x0 + r, y0, color);
  drawPixel(x0 - r, y0, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}


void TFT_LG_ILI9225::fillCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color) {

  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x = 0;
  int16_t y = radius;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawLine(x0 + x, y0 + y, x0 - x, y0 + y, color); // bottom
    drawLine(x0 + x, y0 - y, x0 - x, y0 - y, color); // top
    drawLine(x0 + y, y0 - x, x0 + y, y0 + x, color); // right
    drawLine(x0 - y, y0 - x, x0 - y, y0 + x, color); // left
  }
  fillRectangle(x0-x, y0-y, x0+x, y0+y, color);
}

/**
 * @brief TFT_LG_ILI9225::drawLine
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @param color
 */
void TFT_LG_ILI9225::drawLine(
    uint16_t x0, uint16_t y0,
    uint16_t x1, uint16_t y1,
    uint16_t color
) {
  int x = x1 - x0;
  int y = y1 - y0;
  int dx = abs(x), sx = x0 <= x1 ? 1 : -1;
  int dy = -abs(y), sy = y0 <= y1 ? 1 : -1;
  int err = dx + dy, e2; /* error value e_xy */
  /* loop */
  for (;;) {
    drawPixel(x0, y0, color);
    e2 = 2 * err;
    if (e2 >= dy) {
      /* e_xy+e_x > 0 */
      if (x0 == x1) break;
      err += dy; x0 += sx;
    }
    if (e2 <= dx) {
      /* e_xy+e_y < 0 */
      if (y0 == y1) break;
      err += dx; y0 += sy;
    }
  }
}

void TFT_LG_ILI9225::drawPixel(uint16_t x1, uint16_t y1, uint16_t color) {

  if(x1 >= _maxX || y1 >= _maxY) return;

  _setWindow(x1, y1, x1, y1);
  _writeData(color);
}


uint16_t TFT_LG_ILI9225::maxX() {
  return _maxX;
}


uint16_t TFT_LG_ILI9225::maxY() {
  return _maxY;
}

/*
uint16_t TFT_LG_ILI9225::setColor(uint8_t red8, uint8_t green8, uint8_t blue8) {
  // rgb16 = red5 green6 blue5
  return (red8 >> 3) << 11 | (green8 >> 2) << 5 | (blue8 >> 3);
}
*/
/*
void TFT_LG_ILI9225::splitColor(uint16_t rgb, uint8_t &red, uint8_t &green, uint8_t &blue) {
  // rgb16 = red5 green6 blue5
  red   = (rgb & 0b1111100000000000) >> 11 << 3;
  green = (rgb & 0b0000011111100000) >>  5 << 2;
  blue  = (rgb & 0b0000000000011111)       << 3;
}
*/

void TFT_LG_ILI9225::_swap(int16_t &a, int16_t &b) {
  int16_t w = a;
  a = b;
  b = w;
}

// Utilities
void TFT_LG_ILI9225::_writeCommand(uint16_t command) {
  _writecommand(command >> 8);
  _writecommand(command & 0xFF);
}

void TFT_LG_ILI9225::_writeData(uint16_t data) {
  _writedata(data >> 8);
  _writedata(data & 0xFF);
}

void TFT_LG_ILI9225::_writeRegister(uint16_t reg, uint16_t data) {
  _writeCommand(reg);
  _writeData(data);
}


void TFT_LG_ILI9225::drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color) {
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x3, y3, color);
  drawLine(x3, y3, x1, y1, color);
}


void TFT_LG_ILI9225::fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y3 >= y2 >= y1)
  if (y1 > y2) {
    _swap(y1, y2); _swap(x1, x2);
  }
  if (y2 > y3) {
    _swap(y3, y2); _swap(x3, x2);
  }
  if (y1 > y2) {
    _swap(y1, y2); _swap(x1, x2);
  }

  if (y1 == y3) { // Handle awkward all-on-same-line case as its own thing
    a = b = x1;
    if (x2 < a)      a = x2;
    else if (x2 > b) b = x2;
    if (x3 < a)      a = x3;
    else if (x3 > b) b = x3;
    drawLine(a, y1, b, y1, color);
    return;
  }

  uint16_t  dx11 = x2 - x1,
  dy11 = y2 - y1,
  dx12 = x3 - x1,
  dy12 = y3 - y1,
  dx22 = x3 - x2,
  dy22 = y3 - y2,
  sa   = 0,
  sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y2=y3 (flat-bottomed triangle), the scanline y2
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y2 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y1=y2
  // (flat-topped triangle).
  if (y2 == y3) last = y2;   // Include y2 scanline
  else          last = y2 - 1; // Skip it

  for (y = y1; y <= last; y++) {
    a   = x1 + sa / dy11;
    b   = x1 + sb / dy12;
    sa += dx11;
    sb += dx12;
/*     longhand:
  a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
  b = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
*/
    if (a > b) _swap(a,b);
    drawLine(a, y, b, y, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y2=y3.
  sa = dx22 * (y - y2);
  sb = dx12 * (y - y1);
  for (; y<=y3; y++) {
    a   = x2 + sa / dy22;
    b   = x1 + sb / dy12;
    sa += dx22;
    sb += dx12;
/* longhand:
   a = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
   b = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
*/
    if (a > b) _swap(a,b);
    drawLine(a, y, b, y, color);
  }
}


void TFT_LG_ILI9225::setBackgroundColor(uint16_t color) {
  _bgColor = color;
}

uint16_t TFT_LG_ILI9225::getBackgroundColor() {
  return _bgColor;
}

void TFT_LG_ILI9225::setFontSize(uint8_t fontSize) {
  _fontSize = fontSize;
}

uint8_t TFT_LG_ILI9225::getFontSize() {
  return _fontSize;
}

void TFT_LG_ILI9225::drawText(uint16_t x, uint16_t y, const char * string, uint16_t color) {

  uint16_t currx = x;
  // Print every character in string
  while (*string) {
    currx += drawChar(currx, y, *string++, color);
  };
}

uint8_t TFT_LG_ILI9225::drawChar(
    uint16_t x, uint16_t y, uint8_t ascii, uint16_t color
) {
  uint8_t buf[8];
  memcpy_P(&buf, &russFontANSI[ascii], FONT_X);
  buf[FONT_X] = 0;
  _setWindow(x, y, x + (FONT_X + 1) * _fontSize - 1, y + FONT_Y * _fontSize - 1);
  uint16_t count = 0;
  switch (_orientation) {
    case 0:
      for (int8_t f = 0; f < FONT_Y; f++) {
        for (int8_t ffSize = 0; ffSize < _fontSize; ffSize++) {
          for (int8_t i = 0; i <= FONT_X; i++) {
            uint16_t outColor = ((buf[i] >> f) & 0x01) ? color : _bgColor;
            for (int8_t ifSize = 0; ifSize < _fontSize; ifSize++) {
              _writeData(outColor);
            }
          }
        }
      }
      break;
    case 1:
      for (int8_t i = 0; i <= FONT_X; i++) {
        for (int8_t ifSize = 0; ifSize < _fontSize; ifSize++) {
          for (int8_t f = FONT_Y - 1; f >= 0; f--) {
            uint16_t outColor = ((buf[i] >> f) & 0x01) ? color : _bgColor;
            for (int8_t ffSize = 0; ffSize < _fontSize; ffSize++) {
              _writeData(outColor);
              count++;
            }
          }
        }
      }
      break;
    case 2:
      for (int8_t f = FONT_Y - 1; f >= 0; f--) {
        for (int8_t ifSize = 0; ifSize < _fontSize; ifSize++) {
          for (int8_t i = FONT_X; i >= 0; i--) {
            uint16_t outColor = ((buf[i] >> f) & 0x01) ? color : _bgColor;
            for (int8_t ffSize = 0; ffSize < _fontSize; ffSize++) {
              _writeData(outColor);
            }
          }
        }
      }
    break;
    case 3:
      for (int8_t i = FONT_X; i >= 0; i--) {
        for (int8_t ifSize = 0; ifSize < _fontSize; ifSize++) {
          for (int8_t f = 0; f < FONT_Y; f++) {
            uint16_t outColor = ((buf[i] >> f) & 0x01) ? color : _bgColor;
            for (int8_t ffSize = 0; ffSize < _fontSize; ffSize++) {
              _writeData(outColor);
              count++;
            }
          }
        }
      }
    break;
  }
  return (FONT_X + 1) * _fontSize;
}

// Draw a 1-bit image (bitmap) at the specified (x,y) position from the
// provided bitmap buffer (must be PROGMEM memory) using the specified
// foreground color (unset bits are transparent).
/*void TFT_LG_ILI9225::drawBitmap(
  const uint8_t *bitmap,
  int16_t x, int16_t y, int16_t w, int16_t h,
  uint16_t color
) {

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t bt;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      if (i & 7) {
        bt <<= 1;
      } else {
        bt = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      }
      if (bt & 0x80) drawPixel(x + i, y + j, color);
    }
  }
}*/

// Draw a 1-bit image (bitmap) at the specified (x,y) position from the
// provided bitmap buffer (must be PROGMEM memory) using the specified
// foreground (for set bits) and background (for clear bits) colors.
void TFT_LG_ILI9225::drawBitmap(
    const uint8_t *bitmap,
    int16_t x, int16_t y, int16_t w, int16_t h, int16_t byteWidth,
    uint16_t color
) {
  Size size = _setWindow(x, y, x + w - 1, y + h - 1);
  uint16_t count = size.count();
//  Serial.print("width=");
//  Serial.print(size.width);
//  Serial.print("; height=");
//  Serial.print(size.height);
//  Serial.print("; count=");
//  Serial.print(count);
//  Serial.println(";");

  for (uint16_t r = 0; r < size.height; r++) {
    for (uint16_t c = 0; c < size.width; c++ ) {
      uint16_t col = c, row = r;
      switch (_orientation) {
      case 0:
        break;
      case 1:
        row = size.width - c - 1;
        col = r;
        break;
      case 2:
        col = size.width - c - 1;
        row = size.height - r - 1;
        break;
      case 3:
        col = size.width - r - 1;
        row = c;
        break;
      }
      col = col >= size.width ? size.width - 1 : col;
      row = row >= size.height ? size.height - 1 : row;
      writeBitmapPixel(bitmap, col, row, byteWidth, color);
      if (--count == 0) break;
    }
    if (count == 0) break;
  }
}

void TFT_LG_ILI9225::writeBitmapPixel(
    const uint8_t *bitmap, int16_t x, int16_t y, int16_t byteWidth,
    uint16_t color
) {
  uint8_t _byte = pgm_read_byte(bitmap + y * byteWidth + x / 8);
  uint8_t mode = 0x80 >> (x % 8);
  _writeData((_byte & mode) ? color : _bgColor);
}
