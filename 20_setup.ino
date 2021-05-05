void setup() {

  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(_2posSw, INPUT_PULLUP);
  pinMode(pot, INPUT);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Aquatroller 1.0");
  //delay(800);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CHECKING TIME...");
  delay(1000);

  DateTime now = rtc.now();

  if (checkDaytime()) {

    lcd.setCursor(0, 1);
    lcd.print("DAY MODE");
    lightOn = 1;
    lcd.setBacklight(HIGH);
    uint8_t brightness = 0;
    uint8_t lastPercent;
    delay(1500);
    while (brightness < maxPWMsteps) {
      analogWrite(ledPin, brightness);
      uint16_t percent = map(brightness, 0, maxPWMsteps, 0, 100); //even though the percent var can only go up to 100, it still occupies more memory than 8bits uint8_t so must use 16nit mem allocation.
      if (percent != lastPercent) {
        sprintf(illuminate, "ILLUMINATING %d%%", percent);
        lcd.setCursor(0, 1);
        lcd.print(illuminate);
      }
      lastPercent = percent;
      brightness++;
      delay(fadeDelay);
    }
  } else {
    lcd.setCursor(0, 1);
    lcd.print("NIGHT MODE");
    delay(2000);
    lightOn = 0;
    lcd.setBacklight(LOW);
    updateLCD();
  }
}
