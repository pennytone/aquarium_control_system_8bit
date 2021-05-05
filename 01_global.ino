#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

const int en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3; // Define LCD pinout
LiquidCrystal_I2C lcd(0x27, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);
RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

const uint8_t ledPin = 5;
uint8_t _2posSw = 2;
bool _2posSwValue;
int8_t pot = A6;
uint8_t  maxPWMsteps = 255;
uint16_t fadeDelay = 100;
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
  }
}
