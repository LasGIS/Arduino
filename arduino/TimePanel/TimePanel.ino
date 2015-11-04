#include <Servo.h>

// create servo object to control a servo
Servo myservo;

//Пин подключен к резистору
const int resistorPin = A0;
//Пин подключен к резистору
const int motorPin = 5;

/**  */
String panelValue = "";
volatile long timePanel = 0;
//volatile int IRTestCount = -1;
volatile long IRTestValue = 0;
volatile boolean IRTestHasValue = false;
int showCom = 0;

void setup() {
  Serial.begin(9600);
  myservo.attach(motorPin);  
  initPanel();
  //initIRControl();
  initIRTest();
}

void loop() {
  int resistorValue = analogRead(resistorPin);
  myservo.write(map(resistorValue, 0, 1023, 0, 180));

  switch (showCom) {
    case 0:
      setTimeToPanel();
      break;
    case 1:
      panelValue = String((resistorValue * 180.0) / 1023, 2);
      break;
    case 2:
      panelValue = "-02-";
      //setIRTestToPanel();
      break;
    case 3:
      panelValue = "-03-";
      break;
    case 4:
      panelValue = "-04-";
      break;
    default:
     panelValue = "defa";
     break;
  }

  if (IRTestHasValue) {
    IRTestHasValue = false;
    setIRTestToPanel();

/*    
    Serial.print("bit = ");
    Serial.print(IRTestCount);
*/
    Serial.print("key = ");
    Serial.println(IRTestValue, HEX);

    if (IRTestValue == 0xFFA857) {
      shimmiDance();
    } else if (IRTestValue == 0xFFE01F) {
      myservo.write(180);
    } else if (IRTestValue == 0xFF02FD) {
      myservo.write(90);
    } else if (IRTestValue == 0xFF906F) {
      myservo.write(0);
    } else if (IRTestValue == 0xFFC23D) {
      showCom++;
      if (showCom > 3) {
        showCom = 0;
      }
    }
    delay(1000);
  }
  delay(200);
}

void setTimeToPanel() {
  static int count = 0;
  panelValue = timeToString(count == 0);
  if (count >= 600) {
    //showDoubleOut();
    count = 0;
  } else {
    count++;
  }
}

void setIRTestToPanel() {
  String value = String(IRTestValue, HEX);
  int len = value.length();
  if (len > 4) {
    panelValue = value.substring(len - 4, len);
  } else {
    panelValue = value;
  }
}

void serialEvent() {
  char buf[10];
  int cnt = Serial.readBytes(buf, 10) - 1;
  if (cnt >= 0 && cnt < 10) {
    buf[cnt] = 0;
  }
  panelValue = String(buf);
  Serial.println("\"" + panelValue + "\"");
  //IRTestCheck();
  //IRTestRead();
  //IRTestEvent();
  boolean isDigits = true;
  for (int i = 0; i < cnt; i++) {
    if (!isDigit(buf[i])) {
      isDigits = false;
    }
  }
  if (isDigits) {
    int degre = panelValue.toInt();
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
      panelValue = String(i, DEC);
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
      panelValue = String(i, DEC);
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
    panelValue = String(val, 2);
    delay(100);
  }
  panelValue = "B.0.b.a.";
  delay(10000);
}
