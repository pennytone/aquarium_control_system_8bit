#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
//#include <LightFade.ino>

const int en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3; // Define LCD pinout
LiquidCrystal_I2C lcd(0x27, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

const uint8_t ledPin = 5;
uint8_t _2posSw = 2;
bool _2posSwValue;
int8_t pot = A6;
uint8_t  maxPWMsteps = 254;
uint16_t fadeDelay = 10;
uint8_t  brightness = 0;
uint16_t daytimeStart = 612;
uint16_t daytimeEnd = 1948;
const uint8_t piezoPin = 2;
unsigned long int lastIteration = 0;
bool lightOn;
char illuminate[16];

bool checkDaytime() {
  DateTime now = rtc.now();
  int nowInteger = now.hour() * 100 + now.minute();
  if (nowInteger > daytimeStart && nowInteger < daytimeEnd)
  {
    return 1;
  }
  return 0;
}

void updateLCD(uint16_t interval = 1000) {
  DateTime now = rtc.now();
  if ((millis() - lastIteration) > interval) {
    lastIteration = millis();

    int farenheit = rtc.getTemperature() * 1.8 + 32;
    char buf1[] = "MMM DD YYYY DDD";
    char buf2[] = "hh:mm:ssAP";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(now.toString(buf1));
    lcd.setCursor(0, 1);
    lcd.print(now.toString(buf2));
    lcd.setCursor(11, 1);
    lcd.print(farenheit);
    lcd.setCursor(13, 1);
    lcd.print((char)223); //ascii number for degrees symbol
    lcd.print("F");
    //Serial.println(now.toString(buf2));
  }
}

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
  //delay(1000);

  DateTime now = rtc.now();

  if (checkDaytime()) {

    lcd.setCursor(0, 1);
    lcd.print("DAY MODE");
    //delay(1200);
//    Serial.println("Day Mode");
    lightOn = 1;
    lcd.setBacklight(HIGH);
    uint8_t brightness = 0;
    uint8_t lastPercent;
    //delay(1500);
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

void lightFade(int8_t x) {
//  Serial.print("running lightfade value of x = ");
//  Serial.println(x);
//  Serial.print("brightness = ");
//  Serial.println(brightness);
  char illuminate[16];
  //  uint8_t brightness = 0;
  uint8_t lastPercent;
  while ((brightness > 0) && (brightness < 255))
  {
    brightness = brightness + x;
    analogWrite(ledPin, brightness);

//    Serial.println(brightness);
    if ((x == 1) && (brightness == 140))
    {
      lcd.setBacklight(HIGH);
//      Serial.println("lcd is on");
    }
    else if ((x == -1) && (brightness == 140))
    {
      lcd.setBacklight(LOW);
//      Serial.println("lcd is off");
    }
    else {
      break;
    }

    uint16_t percent = map(brightness, 0, maxPWMsteps, 0, 100);
    if (percent != lastPercent)
    {
      sprintf(illuminate, "ILLUMINATING %d%", percent);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(illuminate);
    }
    lastPercent = percent;

    delay(fadeDelay);
  }
  Serial.println("end of lightfade");
}

void lightFadeOn(uint16_t y = fadeDelay, uint8_t z = maxPWMsteps) {
  char illuminate[16];
//  uint8_t brightness = 0;
  uint8_t lastPercent;
  while ( brightness < z)
  {
    brightness++;
    if (brightness > 140)
    {
      lcd.setBacklight(HIGH);
    }
    uint16_t percent = map(brightness, 0, z, 0, 100);
    if (percent != lastPercent)
    {
      sprintf(illuminate, "ILLUMINATING %d%", percent);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(illuminate);
    }
    lastPercent = percent;
    analogWrite(ledPin, brightness);
    delay(y);
  }
}

void lightFadeOff(uint16_t y = fadeDelay, uint8_t z = maxPWMsteps) {

  char dim[8];
  uint8_t lastPercent;
  uint8_t brightness = z;
  while ( brightness > 0)
  {
    brightness--;
    if (brightness < 140)
    {
      lcd.setBacklight(LOW);
    }

    uint16_t percent = map(brightness, 0, z, 0, 100);
    if (percent != lastPercent)
    {
      sprintf(dim, "DIMMING %d%", percent);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(dim);
    }
    lastPercent = percent;
    analogWrite(ledPin, brightness);
    delay(y);
  }
}


void loop() {
//    Serial.print("lightOn = ");
//    Serial.println(lightOn);
//    Serial.print("brightness = ");
//    Serial.println(brightness);
  _2posSwValue = digitalRead(_2posSw);

  while (_2posSwValue == HIGH)
  {
//    Serial.println("_2posSw loop");
    uint16_t potValue = analogRead(pot);
    potValue = map(potValue, 0, 1024, 0, maxPWMsteps);
    analogWrite(ledPin, potValue);
    if (digitalRead(_2posSw) == LOW) {
      break;
    }
  }
  if (checkDaytime())
  {
//    Serial.println("it's daytime mf-ers");
    switch (lightOn) {
      case 0:
//        Serial.println("daytime & light is currently off, running lightFade(1)");
//        brightness = 1;
        lightFadeOn();
        lightOn = !lightOn;
        break;
      case 1:
//        Serial.println("daytime & light is currently on, keeping light on");
        analogWrite(ledPin, 255);
        break;
    }
  }
  else {
    switch (lightOn) {
      case 1:
//        Serial.println("nighttime & light is currently on, running lightFade(-1)");
//        brightness = 254;
        lightFadeOff();
        lightOn = !lightOn;
        break;
      case 0:
//        Serial.println("nighttime & light is currently off, keeping light off");
        analogWrite(ledPin, 0);
        break;
    }
  }

  updateLCD(1000);
}
