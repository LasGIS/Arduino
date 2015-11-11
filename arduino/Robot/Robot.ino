#include <IrControl.h>
#include <DigitPanel.h>

// указываем пин для ИК датчика 
IrControl control(2);
DigitPanel panel(  
  8,  //Пин подключен к ST_CP входу 74HC595
  12, //Пин подключен к SH_CP входу 74HC595
  11, //Пин подключен к DS входу 74HC595
  new int[4]{10,9,7,4}, // 4 Пина для управления цифрами
  4
);

void setup() {
  Serial.begin(9600);
  control.start();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (control.hasCode()) {
    Serial.print("IR key = ");
    Serial.println(control.getCode(), HEX);
  }
  delay(100);
}
