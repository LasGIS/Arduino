#include <IrControl.h>
#include <DigitPanel.h>

// указываем пин для ИК датчика 
IrControl control(2);
DigitPanel panel(  
  8,  //Пин подключен к ST_CP входу 74HC595
  12, //Пин подключен к SH_CP входу 74HC595
  11, //Пин подключен к DS входу 74HC595
  // 4 Пина для управления цифрами
  new int[4]{10,9,7,4}, 4
);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (control.hasCode()) {
    long code = control.getCode();
    Serial.print("IR key = ");
    Serial.println(control.toKey(code));
    Serial.print("; code = ");
    Serial.println(control.getCode(), HEX);
  }
  delay(100);
}
