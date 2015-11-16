#include <Servo.h>
#include <IrControl.h>
#include <DigitPanel.h>

// create servo object to control a servo
Servo myservo;
// указываем пин для ИК датчика 
IrControl control(2);
DigitPanel panel(  
  8,  //Пин подключен к ST_CP входу 74HC595
  12, //Пин подключен к SH_CP входу 74HC595
  11, //Пин подключен к DS входу 74HC595
  // 4 Пина для управления цифрами
  new int[4]{10,9,7,4}, 4
);

// Пин подключен к резистору
const int resistorPin = A0;
// Пин подключен к сервоприводу
const int motorPin = 5;
// текущая команда
int showCom = 0;

void setup() {
  Serial.begin(9600);
  myservo.attach(motorPin);
}

void loop() {

  switch (showCom) {
    case 0:
      setTimeToPanel();
      break;
    case 1: {
        int resistorValue = analogRead(resistorPin);
        myservo.write(map(resistorValue, 0, 1023, 0, 180));
        panel.setValue(String((resistorValue * 180.0) / 1023, 2));
      }
      break;
    case 2:
      setIRTestToPanel();
      break;
    case 3:
      showBox();
      break;
    case 4:
      panel.setValue("-44-");
      break;
    default:
      panel.setValue("defa");
      break;
  }

  if (control.hasCode()) {
    long code = control.getCode();
    char key = control.toKey(code);
    Serial.print("IR key = ");
    Serial.print(key);
    Serial.print("; code = ");
    Serial.println(control.code, HEX);

    switch (key) {
      case 's':
        shimmiDance();
      break;
      case 'r':
        myservo.write(180);
        delay(1000);
      break;
      case 'f':
        myservo.write(90);
        delay(1000);
      break;
      case 'l':
        myservo.write(0);
        delay(1000);
      break;
      case 'b':
        showCom++;
        if (showCom > 5) {
          showCom = 0;
        } else if (showCom == 3) {
          for (int i = 0; i < 4; i++) DigitPanel::panel[i] = 0;
        }
      break;
      case 't':
        showCom--;
        if (showCom < 0) {
          showCom = 5;
        } else if (showCom == 3) {
          for (int i = 0; i < 4; i++) DigitPanel::panel[i] = 0;
        }
      break;
    }
  }
  delay(100);
}

void setTimeToPanel() {
  static int count = 0;
  panel.setValue(timeToString(count == 0));
  if (count >= 600) {
    //showDoubleOut();
    count = 0;
  } else {
    count++;
  }
}

void setIRTestToPanel() {
  String value = String(control.code, HEX);
  int len = value.length();
  if (len > 4) {
    panel.setValue(value.substring(len - 4, len));
  } else {
    panel.setValue(value);
  }
}

void serialEvent() {
  char buf[10];
  int cnt = Serial.readBytes(buf, 10) - 1;
  if (cnt >= 0 && cnt < 10) {
    buf[cnt] = 0;
  }
  panel.setValue(String(buf));
  Serial.println("\"" + panel.getValue() + "\"");
  boolean isDigits = true;
  for (int i = 0; i < cnt; i++) {
    if (!isDigit(buf[i])) {
      isDigits = false;
    }
  }
  if (isDigits) {
    int degre = panel.getValue().toInt();
    if (degre >= 0 && degre <= 180) {
      myservo.write(degre);
    }
  } else {
    shimmiDance();
  }

  delay(2000);
}

void shimmiDance() {
  int i = myservo.read();
  for (int j = 0; j < 3; j++) {
    for (i++; i <= 180; i++) {
      panel.setValue(String(i, DEC));
      myservo.write(i);
      //Serial.println(i);
      if (i % 90 == 0) {
        delay(1000);
      } else if (i % 30 == 0) {
        delay(300);
      } else {
        delay(30);
      }
    }
    for (i--; i >= 0; i--) {
      panel.setValue(String(i, DEC));
      myservo.write(i);
      //Serial.println(i);
      if (i % 90 == 0) {
        delay(1000);
      } else if (i % 30 == 0) {
        delay(300);
      } else {
        delay(30);
      }
    }
  }
}

const byte showBoxData[] = {
  0, B00000100, 1, B00000100, 2, B00000100, 3, B00000100, 3, B00100000, 3, B01000000, 
  3, B00010000, 2, B00010000, 1, B00010000, 0, B00010000, 0, B00000010,
  0, B00001000, 1, B00001000, 2, B00001000, 3, B00001000
};

void showBox() {
  static int pos = 0;
  static bool sets = true;
  byte b = DigitPanel::panel[showBoxData[pos]];
  if (sets) {
    b = b | (showBoxData[pos + 1] & B11111111);
  } else {
    b = b ^ (showBoxData[pos + 1] & B11111111);
  }
  DigitPanel::panel[showBoxData[pos]] = b;  
  pos += 2;
  if (pos == 30) {
    pos = 0;
    sets = sets ? false : true;
  }
}

String timeToString(boolean isSerialPrint) {
  unsigned long time = millis();
  boolean halfsec = (time % 1000) < 500;
  time /= 1000;
  int sec = time % 60;
  time /= 60;
  int min = time % 60;
  time /= 60;
  int hour = time;
  if (isSerialPrint) {
    Serial.print("hour = ");
    Serial.print(hour);
    Serial.print("; min = ");
    Serial.print(min);
    Serial.print("; sec = ");
    Serial.print(sec);
    Serial.println(";");
  }
  if (hour > 0) {
    return toTwo(hour) + (halfsec ? "." : "") + toTwo(min);// + ((sec & 1) ? "." : "");
  } else {
    return toTwo(min) + (halfsec ? "." : "") + toTwo(sec);// + ((sec & 1) ? "." : "");
  }
}

String toTwo(int val) {
  String str = "00";
  str = str + val;
  return str.substring(str.length() - 2);
}

void showDoubleOut() {
  for (double val = -11.0; val < 11.0; val += .01) {
    panel.setValue(String(val, 2));
    delay(100);
  }
  panel.setValue("B.0.b.a.");
  delay(10000);
}
