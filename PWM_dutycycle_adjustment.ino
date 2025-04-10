#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Button pins
const int btnIncrease = 2;
const int btnDecrease = 3;

// PWM Output
const int pwmPin = 9; // Timer1

// Frequency and Duty
const int frequency = 1000;   // Hz (fixed)
int dutyCycle = 50;           // %

unsigned long lastDebounce = 0;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(btnIncrease, INPUT_PULLUP);
  pinMode(btnDecrease, INPUT_PULLUP);

  setupPWM(frequency, dutyCycle);
  updateLCD();
}

void loop() {
  // Debounce time
  if (millis() - lastDebounce > 200) {
    if (digitalRead(btnIncrease) == LOW) {
      dutyCycle += 5;
      dutyCycle = constrain(dutyCycle, 0, 100);
      setupPWM(frequency, dutyCycle);
      updateLCD();
      lastDebounce = millis();
    }
    if (digitalRead(btnDecrease) == LOW) {
      dutyCycle -= 5;
      dutyCycle = constrain(dutyCycle, 0, 100);
      setupPWM(frequency, dutyCycle);
      updateLCD();
      lastDebounce = millis();
    }
  }
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Freq: ");
  lcd.print(frequency);
  lcd.print(" Hz");

  lcd.setCursor(0, 1);
  lcd.print("Duty: ");
  lcd.print(dutyCycle);
  lcd.print("%");
}

void setupPWM(int freq, int duty) {
  pinMode(pwmPin, OUTPUT);

  // Stop Timer1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  // Prescaler settings
  int prescalerVal = 1;
  int prescalerBits = 1;

  long top = (16000000 / (prescalerVal * freq)) - 1;
  while (top > 65535 && prescalerBits < 5) {
    prescalerBits++;
    switch (prescalerBits) {
      case 2: prescalerVal = 8; break;
      case 3: prescalerVal = 64; break;
      case 4: prescalerVal = 256; break;
      case 5: prescalerVal = 1024; break;
    }
    top = (16000000 / (prescalerVal * freq)) - 1;
  }

  ICR1 = top;
  OCR1A = (duty * top) / 100;

  TCCR1A = (1 << COM1A1) | (1 << WGM11);           // Non-inverting mode
  TCCR1B = (1 << WGM13) | (1 << WGM12) | prescalerBits; // Fast PWM, mode 14
}






// PWM Output
const int pwmPin = 5; // Timer1

// Frequency and Duty
int dutyCycle = 127;           // 

void setup() {
  //pinMode(6,OUTPUT);

pinMode(5,OUTPUT);
}
void loop()
{
digitalWrite(5,HIGH);
delayMicroseconds(400);
digitalWrite(5,LOW);
delayMicroseconds(1000-400);
}





