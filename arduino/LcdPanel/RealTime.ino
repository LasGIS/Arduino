String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: { return "Sun"; }
    case Time::kMonday: return "Mon";
    case Time::kTuesday: return "Tue";
    case Time::kWednesday: return "Wed";
    case Time::kThursday: return "Thu";
    case Time::kFriday: return "Fri";
    case Time::kSaturday: return "Sat";
  }
  return "(und)";
}

void printTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%s %04d-%02d-%02d ", day.c_str(), t.yr, t.mon, t.date);
  lcd.setCursor(0, 0);
  lcd.print(buf);
  Serial.print(buf);
  snprintf(buf, sizeof(buf), "Time = %02d:%02d:%02d", t.hr, t.min, t.sec);
  lcd.setCursor(0, 1);
  lcd.print(buf);
  Serial.println(buf);
//    lcd.setCursor(0, 0);
//    lcd.print("Time = ");
//    lcd.print(timeToString(milliSec));

}

