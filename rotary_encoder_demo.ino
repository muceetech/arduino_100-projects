#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>


// LCD config
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust 0x27 if needed

Encoder myEnc(5,6);

char buf[17];

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Encoder Value:");

}

long oldPosition  = -999;

void loop() {
 //lcd.clear();
 //delay(100);
long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    int newPos = newPosition/2;
    oldPosition = newPos;
    sprintf(buf, "%-5d", newPos); // left-align with 5-character field
    lcd.setCursor(0, 1);
    lcd.print(buf);
  }
}

