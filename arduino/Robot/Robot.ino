#include <MsTimer2.h>
#include <IrControl.h>

// указываем пин для ИК датчика 
IrControl control(2);

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
