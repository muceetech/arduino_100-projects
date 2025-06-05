#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
const int btnPins[] = {4, 5, 6, 7};
const int rxPin = 2;
volatile unsigned long pulseStart = 0;
volatile unsigned long duration = 0;
volatile bool pulseReceived = false;

void setup() {
  pinMode(rxPin, INPUT);
   for (int i = 0; i < 4; i++) {
    pinMode(btnPins[i], OUTPUT);
  }
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  attachInterrupt(digitalPinToInterrupt(rxPin), handleChange, CHANGE);
}

void loop() {
  //unsigned long duration = pulseIn(rxPin, HIGH, 30000); // 30ms timeout

  if (duration > 100) {
    String button;
    if ((duration > 200) && (duration < 750)) button = "1";
    else if ((duration > 750) && (duration < 1200)) button = "2";
    else if ((duration > 1200) && (duration < 1750)) button = "4";
    else if ((duration > 1750) && (duration < 2250)) button = "3";
    else button = "0";

    lcd.setCursor(0,0);
    lcd.print("Pulse: ");
    lcd.print(duration);
    lcd.print(" us ");
    lcd.setCursor(0,14);
    lcd.print(button);
    delay(200); // debounce
    if(button == "1")   // Forward
    {
      digitalWrite(4,HIGH);
      digitalWrite(5,LOW);
      digitalWrite(6,HIGH);
      digitalWrite(7,LOW);
    }
    if(button == "2")   // right
    {
      digitalWrite(4,HIGH);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      digitalWrite(7,LOW);
    }
    if(button == "3")   // left
    {
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,HIGH);
      digitalWrite(7,LOW);
    }
    if(button == "4")  // reverse
    {
      digitalWrite(4,LOW);
      digitalWrite(5,HIGH);
      digitalWrite(6,LOW);
      digitalWrite(7,HIGH);
    }
    if(button == "0")   // no button is pressed
    {
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);
      digitalWrite(6,LOW);
      digitalWrite(7,LOW);
    }
  }
}

void handleChange() {
  bool state = digitalRead(rxPin);

  if (state == HIGH) {
    // Pulse started
    pulseStart = micros();
  } else {
    // Pulse ended
    duration = micros() - pulseStart;
    pulseReceived = true;
  }
}
