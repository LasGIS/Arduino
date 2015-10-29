/**  */
String panelValue = "";
volatile long timePanel = 0;

void setup() {
  Serial.begin(9600);
  initPanel();
  //initIRControl();
  initIRTest();
}

void loop() {
  static int count = 0;
  panelValue = timeToString(count == 0);
  if (count >= 600) {
    //showDoubleOut();
    count = 0;
  } else {
    count++;
  }
  //checkIRControl();
  delay(100);
  startIRTest();
}

void serialEvent() {
  char buf[10];
  int cnt = Serial.readBytes(buf, 10) - 1;
  if (cnt >= 0 && cnt < 10) {
    buf[cnt] = 0;
  }
  panelValue = String(buf);
  Serial.println("\"" + panelValue + "\"");
  IRTestCheck();
  //IRTestRead();
  //IRTestEvent();
  delay(5000);
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
