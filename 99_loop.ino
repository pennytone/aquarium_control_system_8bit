void loop() {
  _2posSwValue = digitalRead(_2posSw);

  while (_2posSwValue == HIGH)
  {
    uint16_t potValue = analogRead(pot);
    potValue = map(potValue, 0, 1024, 0, maxPWMsteps);
    analogWrite(ledPin, potValue);
    if (digitalRead(_2posSw) == LOW) {
      break;
    }
  }
  if (checkDaytime())
  {
    switch (lightOn) {
      case 0:
        lightFadeOn();
        lightOn = !lightOn;
        break;
      case 1:
        analogWrite(ledPin, 255);
        break;
    }
  }
  else {
    switch (lightOn) {
      case 1:
        lightFadeOff();
        lightOn = !lightOn;
        break;
      case 0:
        analogWrite(ledPin, 0);
        break;
    }
  }

  updateLCD(1000);
}
