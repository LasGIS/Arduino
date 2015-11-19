/*
 */
#include <IrControl.h>
#include <LiquidCrystal.h>

// указываем пин для ИК датчика 
IrControl control(2);
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 10, 6, 7, 8, 9);

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Start");
  control.start();
}

void loop() {
/*  int thisChar = 0;
  for (int row = 0; row < 16; row++) {
    lcd.setCursor(0, row % 2);
    for (int col = 0; col < 16; col++) {
      lcd.write(thisChar++);
    }
    if (row % 2 == 1) {*/
//  Serial.print(".");

  if (control.hasCode()) {
    long code = control.getCode();
    char key = control.toKey(code);
    Serial.print("IR key = ");
    if (key == 0) {
      Serial.print("NOTHIN");
    } else {
      Serial.print(key);
    }
    Serial.print("; code = ");
    Serial.println(control.code, HEX);
    lcd.setCursor(0, 0);
    lcd.print("IR key = ");
    if (key == 0) {
      lcd.print("NOTHIN");
    } else {
      lcd.print(key);
      lcd.print("       ");
    }
    lcd.setCursor(0, 1);
    lcd.print("code = ");
    lcd.print(control.code, HEX);
    lcd.print("       ");
//    delay(1000);
  }
  delay(100);
/*    }
  }*/

/*
    switch (key) {
      case 's':
      break;
      case 'r':
      break;
      case 'f':
      break;
      case 'l':
      break;
      case 'b':
        beforeCommandSet();
        showCom++;
        if (showCom > 5) {
          showCom = 0;
        }
        afterCommandSet();
      break;
      case 't':
        beforeCommandSet();
        showCom--;
        if (showCom < 0) {
          showCom = 5;
        }
        afterCommandSet();
      break;
    }
*/
}
