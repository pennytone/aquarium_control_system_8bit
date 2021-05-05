void lightFade(int8_t x) {

  char illuminate[16];
  uint8_t lastPercent;
  while ((brightness > 0) && (brightness < 255))
  {
    brightness = brightness + x;
    analogWrite(ledPin, brightness);

    if ((x == 1) && (brightness == 140))
    {
      lcd.setBacklight(HIGH);
    }
    else if ((x == -1) && (brightness == 140))
    {
      lcd.setBacklight(LOW);
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
