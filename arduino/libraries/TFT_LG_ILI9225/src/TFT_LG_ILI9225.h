#ifndef TFT_LG_ILI9225_h
#define TFT_LG_ILI9225_h

#define FONT_FROM_I2C_EEPROM

#include <Arduino.h>
#ifdef FONT_FROM_I2C_EEPROM
  #include <I2C_EEPROM.h>

  #define AT24DEVICE 0x57
  #define Font_russFontANSI_5x8 0x0705
  #define FONT_SPACE 6
  #define FONT_X 5
  #define FONT_Y 8
#else
  #include <avr/pgmspace.h>
  #include <RussFontANSI.c>
#endif

/* ILI9225 screen size */
#define ILI9225_LCD_WIDTH  176
#define ILI9225_LCD_HEIGHT 220

//#define __LINKIT_ONE__

/* Pins operation DEFINITION */
#define TFT_RST 8
#define TFT_RS  9
#define TFT_CS  10  // SS
#define TFT_SDI 11  // MOSI
#define TFT_CLK 13  // SCK
//#define TFT_LED 0   // 0 if wired to +5V directly

#if defined(__LINKIT_ONE__)

#define TFT_DC_LOW      digitalWrite(TFT_RS, LOW)
#define TFT_DC_HIGH     digitalWrite(TFT_RS, HIGH)
#define TFT_CS_LOW      digitalWrite(TFT_CS, LOW)
#define TFT_CS_HIGH     digitalWrite(TFT_CS, HIGH)
//#define TFT_BL_OFF      digitalWrite(TFT_LED, LOW)
//#define TFT_BL_ON       digitalWrite(TFT_LED, HIGH)
#define TFT_RST_OFF     digitalWrite(TFT_RST, HIGH)
#define TFT_RST_ON      digitalWrite(TFT_RST, LOW)

#else

#define TFT_CS_LOW  {DDRB |= 0x04;PORTB &=~ 0x04;}
#define TFT_CS_HIGH {DDRB |= 0x04;PORTB |=  0x04;}
#define TFT_DC_LOW  {DDRB |= 0x02;PORTB &=~ 0x02;}
#define TFT_DC_HIGH {DDRB |= 0x02;PORTB |=  0x02;}
//#define TFT_BL_OFF  {DDRD |= 0x80;PORTD &=~ 0x80;}
//#define TFT_BL_ON   {DDRD |= 0x80;PORTD |=  0x80;}
#define TFT_RST_OFF {DDRB |= 0x01;PORTB |=  0x01;}
#define TFT_RST_ON  {DDRB |= 0x01;PORTB &=~ 0x01;}

#endif

/* ILI9225 LCD Registers */
#define ILI9225_DRIVER_OUTPUT_CTRL      (0x01u)  // Driver Output Control
#define ILI9225_LCD_AC_DRIVING_CTRL     (0x02u)  // LCD AC Driving Control
#define ILI9225_ENTRY_MODE              (0x03u)  // Entry Mode
#define ILI9225_DISP_CTRL1              (0x07u)  // Display Control 1
#define ILI9225_BLANK_PERIOD_CTRL1      (0x08u)  // Blank Period Control
#define ILI9225_FRAME_CYCLE_CTRL        (0x0Bu)  // Frame Cycle Control
#define ILI9225_INTERFACE_CTRL          (0x0Cu)  // Interface Control
#define ILI9225_OSC_CTRL                (0x0Fu)  // Osc Control
#define ILI9225_POWER_CTRL1             (0x10u)  // Power Control 1
#define ILI9225_POWER_CTRL2             (0x11u)  // Power Control 2
#define ILI9225_POWER_CTRL3             (0x12u)  // Power Control 3
#define ILI9225_POWER_CTRL4             (0x13u)  // Power Control 4
#define ILI9225_POWER_CTRL5             (0x14u)  // Power Control 5
#define ILI9225_VCI_RECYCLING           (0x15u)  // VCI Recycling
#define ILI9225_RAM_ADDR_SET1           (0x20u)  // Horizontal GRAM Address Set
#define ILI9225_RAM_ADDR_SET2           (0x21u)  // Vertical GRAM Address Set
#define ILI9225_GRAM_DATA_REG           (0x22u)  // GRAM Data Register
#define ILI9225_GATE_SCAN_CTRL          (0x30u)  // Gate Scan Control Register
#define ILI9225_VERTICAL_SCROLL_CTRL1   (0x31u)  // Vertical Scroll Control 1 Register
#define ILI9225_VERTICAL_SCROLL_CTRL2   (0x32u)  // Vertical Scroll Control 2 Register
#define ILI9225_VERTICAL_SCROLL_CTRL3   (0x33u)  // Vertical Scroll Control 3 Register
#define ILI9225_PARTIAL_DRIVING_POS1    (0x34u)  // Partial Driving Position 1 Register
#define ILI9225_PARTIAL_DRIVING_POS2    (0x35u)  // Partial Driving Position 2 Register
#define ILI9225_HORIZONTAL_WINDOW_ADDR1 (0x36u)  // Horizontal Address Start Position
#define ILI9225_HORIZONTAL_WINDOW_ADDR2 (0x37u)  // Horizontal Address End Position
#define ILI9225_VERTICAL_WINDOW_ADDR1   (0x38u)  // Vertical Address Start Position
#define ILI9225_VERTICAL_WINDOW_ADDR2   (0x39u)  // Vertical Address End Position
#define ILI9225_GAMMA_CTRL1             (0x50u)  // Gamma Control 1
#define ILI9225_GAMMA_CTRL2             (0x51u)  // Gamma Control 2
#define ILI9225_GAMMA_CTRL3             (0x52u)  // Gamma Control 3
#define ILI9225_GAMMA_CTRL4             (0x53u)  // Gamma Control 4
#define ILI9225_GAMMA_CTRL5             (0x54u)  // Gamma Control 5
#define ILI9225_GAMMA_CTRL6             (0x55u)  // Gamma Control 6
#define ILI9225_GAMMA_CTRL7             (0x56u)  // Gamma Control 7
#define ILI9225_GAMMA_CTRL8             (0x57u)  // Gamma Control 8
#define ILI9225_GAMMA_CTRL9             (0x58u)  // Gamma Control 9
#define ILI9225_GAMMA_CTRL10            (0x59u)  // Gamma Control 10

#define ILI9225C_INVOFF  0x20
#define ILI9225C_INVON   0x21

/* RGB 24-bits color table definition (RGB888). */
#define RGB888_RGB565(color) (((color) >> 8 & 0xf800) | ((color) >> 5 & 0x07e0) | ((color) >> 3 & 0x001f))

#define COLOR_BLACK          RGB888_RGB565(0x000000u)
#define COLOR_WHITE          RGB888_RGB565(0xFFFFFFu)
#define COLOR_BLUE           RGB888_RGB565(0x0000FFu)
#define COLOR_GREEN          RGB888_RGB565(0x00FF00u)
#define COLOR_RED            RGB888_RGB565(0xFF0000u)
#define COLOR_NAVY           RGB888_RGB565(0x000080u)
#define COLOR_DARKBLUE       RGB888_RGB565(0x00008Bu)
#define COLOR_DARKGREEN      RGB888_RGB565(0x006400u)
#define COLOR_DARKCYAN       RGB888_RGB565(0x008B8Bu)
#define COLOR_CYAN           RGB888_RGB565(0x00FFFFu)
#define COLOR_TURQUOISE      RGB888_RGB565(0x40E0D0u)
#define COLOR_INDIGO         RGB888_RGB565(0x4B0082u)
#define COLOR_DARKRED        RGB888_RGB565(0x800000u)
#define COLOR_OLIVE          RGB888_RGB565(0x808000u)
#define COLOR_GRAY           RGB888_RGB565(0x808080u)
#define COLOR_SKYBLUE        RGB888_RGB565(0x87CEEBu)
#define COLOR_BLUEVIOLET     RGB888_RGB565(0x8A2BE2u)
#define COLOR_LIGHTGREEN     RGB888_RGB565(0x90EE90u)
#define COLOR_DARKVIOLET     RGB888_RGB565(0x9400D3u)
#define COLOR_YELLOWGREEN    RGB888_RGB565(0x9ACD32u)
#define COLOR_BROWN          RGB888_RGB565(0xA52A2Au)
#define COLOR_DARKGRAY       RGB888_RGB565(0xA9A9A9u)
#define COLOR_SIENNA         RGB888_RGB565(0xA0522Du)
#define COLOR_LIGHTBLUE      RGB888_RGB565(0xADD8E6u)
#define COLOR_GREENYELLOW    RGB888_RGB565(0xADFF2Fu)
#define COLOR_SILVER         RGB888_RGB565(0xC0C0C0u)
#define COLOR_LIGHTGREY      RGB888_RGB565(0xD3D3D3u)
#define COLOR_LIGHTCYAN      RGB888_RGB565(0xE0FFFFu)
#define COLOR_VIOLET         RGB888_RGB565(0xEE82EEu)
#define COLOR_AZUR           RGB888_RGB565(0xF0FFFFu)
#define COLOR_BEIGE          RGB888_RGB565(0xF5F5DCu)
#define COLOR_MAGENTA        RGB888_RGB565(0xFF00FFu)
#define COLOR_TOMATO         RGB888_RGB565(0xFF6347u)
#define COLOR_GOLD           RGB888_RGB565(0xFFD700u)
#define COLOR_ORANGE         RGB888_RGB565(0xFFA500u)
#define COLOR_SNOW           RGB888_RGB565(0xFFFAFAu)
#define COLOR_YELLOW         RGB888_RGB565(0xFFFF00u)

class Size {
public:
  uint16_t width;
  uint16_t height;
  Size(uint16_t, uint16_t);
  uint16_t count();
};

/// Main and core class
class TFT_LG_ILI9225 {

  public:

    /// Initialization
    void begin(void);

    /// Clear the screen
    void clear(void);

    /// Invert screen
    /// @param  flag true to invert, false for normal screen
    void invert(boolean flag);

    /// Switch backlight on or off
    /// @param  flag true=on, false=off
//    void setBacklight(boolean flag);

    /// Switch display on or off
    /// @param  flag true=on, false=off
//    void setDisplay(boolean flag);

    /// Set orientation
    /// @param  orientation orientation, 0=portrait, 1=right rotated landscape, 2=reverse portrait, 3=left rotated landscape
    void setOrientation(uint8_t orientation);

    /// Get orientation
    /// @return  orientation orientation, 0=portrait, 1=right rotated landscape, 2=reverse portrait, 3=left rotated landscape
    uint8_t getOrientation(void);

    /// Screen size, x-axis
    /// @return  horizontal size of the screen, in pixels
    /// @note  240 means 240 pixels and thus 0..239 coordinates (decimal)
    uint16_t maxX(void);

    /// Screen size, y-axis
    /// @return  vertical size of the screen, in pixels
    /// @note  220 means 220 pixels and thus 0..219 coordinates (decimal)
    uint16_t maxY(void);

    /// Draw circle
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param color 16-bit color
    void drawCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);

    /// Draw solid circle
    /// @param  x0 center, point coordinate, x-axis
    /// @param  y0 center, point coordinate, y-axis
    /// @param  radius radius
    /// @param  color 16-bit color
    void fillCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color);

    /// Set background color
    /// @param  color background color, default=black
    void setBackgroundColor(uint16_t color = COLOR_BLACK);

    /// Get background color
    /// @return background color
    uint16_t getBackgroundColor();

    /// Draw line, rectangle coordinates
    /// @param  x1 top left coordinate, x-axis
    /// @param  y1 top left coordinate, y-axis
    /// @param  x2 bottom right coordinate, x-axis
    /// @param  y2 bottom right coordinate, y-axis
    /// @param  color 16-bit color
    void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

    /// Draw rectangle, rectangle coordinates
    /// @param  x1 top left coordinate, x-axis
    /// @param  y1 top left coordinate, y-axis
    /// @param  x2 bottom right coordinate, x-axis
    /// @param  y2 bottom right coordinate, y-axis
    /// @param  color 16-bit color
    void drawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

    /// Draw solid rectangle, rectangle coordinates
    /// @param  x1 top left coordinate, x-axis
    /// @param  y1 top left coordinate, y-axis
    /// @param  x2 bottom right coordinate, x-axis
    /// @param  y2 bottom right coordinate, y-axis
    /// @param  color 16-bit color
    void fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

    /// Draw pixel
    /// @param  x1 point coordinate, x-axis
    /// @param  y1 point coordinate, y-axis
    /// @param  color 16-bit color
    void drawPixel(uint16_t x1, uint16_t y1, uint16_t color);

    /// Draw ASCII Text (pixel coordinates)
    /// @param  x point coordinate, x-axis
    /// @param  y point coordinate, y-axis
    /// @param  s text string
    /// @param  color 16-bit color, default=white
    void drawText(uint16_t x, uint16_t y, const char * s, uint16_t color = COLOR_WHITE);

    /// Calculate 16-bit color from 8-bit Red-Green-Blue components
    /// @param  red red component, 0x00..0xff
    /// @param  green green component, 0x00..0xff
    /// @param  blue blue component, 0x00..0xff
    /// @return  16-bit color
//    uint16_t setColor(uint8_t red, uint8_t green, uint8_t blue);

    /// Calculate 8-bit Red-Green-Blue components from 16-bit color
    /// @param  rgb 16-bit color
    /// @param  red red component, 0x00..0xff
    /// @param  green green component, 0x00..0xff
    /// @param  blue blue component, 0x00..0xff
//    void splitColor(uint16_t rgb, uint8_t &red, uint8_t &green, uint8_t &blue);

    /// Draw triangle, triangle coordinates
    /// @param  x1 corner 1 coordinate, x-axis
    /// @param  y1 corner 1 coordinate, y-axis
    /// @param  x2 corner 2 coordinate, x-axis
    /// @param  y2 corner 2 coordinate, y-axis
    /// @param  x3 corner 3 coordinate, x-axis
    /// @param  y3 corner 3 coordinate, y-axis
    /// @param  color 16-bit color
    void drawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t color);

    /// Draw solid triangle, triangle coordinates
    /// @param  x1 corner 1 coordinate, x-axis
    /// @param  y1 corner 1 coordinate, y-axis
    /// @param  x2 corner 2 coordinate, x-axis
    /// @param  y2 corner 2 coordinate, y-axis
    /// @param  x3 corner 3 coordinate, x-axis
    /// @param  y3 corner 3 coordinate, y-axis
    /// @param  color 16-bit color
    void fillTriangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color);

    /// Set current font
    /// @param fontSize Font Size
    void setFontSize(uint8_t fontSize);

    /// Set current font
    /// @return Font Size
    uint8_t getFontSize();

    /// Draw single character (pixel coordinates)
    /// @param  x point coordinate, x-axis
    /// @param  y point coordinate, y-axis
    /// @param  ch ASCII character
    /// @param  color 16-bit color, default=white
    uint8_t drawChar(uint16_t x, uint16_t y, uint8_t ch, uint16_t color = COLOR_WHITE);

    /// Draw bitmap
    /// @param  x point coordinate, x-axis
    /// @param  y point coordinate, y-axis
    /// @param  bitmap
    /// @param  w width
    /// @param  h height
    /// @param  color 16-bit color, default=white
    /// @param  bg 16-bit color, background
//    void drawBitmap(const uint8_t *bitmap, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawBitmap(const uint8_t *bitmap, int16_t x, int16_t y, int16_t w, int16_t h, int16_t byteWidth, uint16_t color);
    void writeBitmapPixel(const uint8_t *bitmap, int16_t x, int16_t y, int16_t byteWidth, uint16_t color);

  private:

    void _writecommand(uint8_t c);
    void _writedata(uint8_t d);

    void _swap(int16_t &a, int16_t &b);
    Size _setWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
    void _orientCoordinates(int16_t &x1, int16_t &y1);
    void _writeRegister(uint16_t reg, uint16_t data);
    void _writeCommand(uint16_t command);
    void _writeData(uint16_t data);

    uint16_t _maxX, _maxY, _bgColor;
    uint8_t _orientation;
    uint8_t _fontSize = 1;

};

#endif
