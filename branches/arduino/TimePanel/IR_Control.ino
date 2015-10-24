/*
#include <IRremote.h>
#include <IRremoteInt.h>

IRrecv irrecv(2); // указываем вывод, к которому подключен приемник
decode_results results;

#define valid_Keys_IR_Control_size 7
int validKeysIRControl[] = {0xFF6897, 0xFF30CF, 0xFF18E7, 0xFF7A85, 0xFF10EF, 0xFF38C7, 0xFF5AA5};
//, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x};

int keyIRControl = -1;

void initIRControl() {
  irrecv.enableIRIn(); // запускаем прием
}

unsigned long checkIRControl() {
  if (irrecv.decode(&results)) { // если данные пришли
    Serial.println(results.value, HEX); // печатаем данные
    for (int key = 0; key < valid_Keys_IR_Control_size; key++) {
      if (results.value == validKeysIRControl[key]) {
        keyIRControl = key;
        break;
      }
    }
    irrecv.resume(); // принимаем следующую команду
  }
}
*/
