//YWROBOT
//Compatible with the Arduino IDE 1.0
//Library version:1.1
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3f, 16, 2);

void setup()
{
  Serial.begin(9600);
  // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, world!");
  lcd.setCursor(0,1);
  lcd.print("Ywrobot Arduino!");
}

void loop() {
  static char chr = 'A';
  lcd.write(chr);
  Serial.write(chr);
  chr++;
  if (chr > 'Z') chr = 'A';
  delay(1000);
}
