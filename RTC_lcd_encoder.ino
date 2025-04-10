#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Encoder.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// RTC
RTC_DS1307 rtc;

// Rotary Encoder Pins
const int pinCLK = 5;
const int pinDT  = 6;
const int pinSW  = 4;

// Encoder object
Encoder knob(pinCLK, pinDT);

// Mode logic
enum Mode { CLOCK, SET_HOUR, SET_MINUTE };
Mode mode = CLOCK;

// Variables
long lastPosition = -999;
int setHour = 0, setMinute = 0;
DateTime currentTime;

void setup() {
  lcd.init();
  lcd.backlight();

  pinMode(pinSW, INPUT_PULLUP);

  Wire.begin();
  if (!rtc.begin()) {
    lcd.print("RTC not found!");
    while (1);
  }

  if (!rtc.isrunning()) {
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    lcd.print("RTC not running!");
  }
}

void loop() {
  static unsigned long lastUpdate = 0;
  static bool lastButtonState = HIGH;

  // --- Handle encoder button press ---
  bool buttonState = digitalRead(pinSW);
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50); // debounce
    if (digitalRead(pinSW) == LOW) {
      switch (mode) {
        case CLOCK:
          currentTime = rtc.now();
          setHour = currentTime.hour();
          setMinute = currentTime.minute();
          knob.write(setHour * 4);
          mode = SET_HOUR;
          break;
        case SET_HOUR:
          knob.write(setMinute * 4);
          mode = SET_MINUTE;
          break;
        case SET_MINUTE:
          rtc.adjust(DateTime(currentTime.year(), currentTime.month(), currentTime.day(), setHour, setMinute, 0));
          mode = CLOCK;
          break;
      }
    }
  }
  lastButtonState = buttonState;

  // --- Read encoder if in setting mode ---
  long newPos = knob.read() / 4;
  if (newPos != lastPosition) {
    lastPosition = newPos;
    if (mode == SET_HOUR) {
      setHour = constrain(newPos, 0, 23);
    } else if (mode == SET_MINUTE) {
      setMinute = constrain(newPos, 0, 59);
    }
    updateDisplay();
  }

  // --- Refresh every second in CLOCK mode ---
  if (mode == CLOCK && millis() - lastUpdate >= 1000) {
    lastUpdate = millis();
    updateDisplay();
  }
}

void updateDisplay() {
  lcd.clear();

  // Get time depending on mode
  int hour, minute, second;
  if (mode == CLOCK) {
    currentTime = rtc.now();
    hour = currentTime.hour();
    minute = currentTime.minute();
    second = currentTime.second();
  } else {
    currentTime = rtc.now(); // just for the date info
    hour = setHour;
    minute = setMinute;
    second = 0;
  }

  // Format time in 12h with AM/PM
  bool isPM = hour >= 12;
  int displayHour = hour % 12;
  if (displayHour == 0) displayHour = 12;

  char timeStr[9];
  sprintf(timeStr, "%02d:%02d:%02d", displayHour, minute, second);

  lcd.setCursor(0, 1);

  // Invert style by inserting space or brackets
  if (mode == SET_HOUR) {
    lcd.print(">");
    lcd.print(displayHour < 10 ? "0" : "");
    lcd.print(displayHour);
    lcd.print("<:");
  } else {
    lcd.print(displayHour < 10 ? "0" : "");
    lcd.print(displayHour);
    lcd.print(":");
  }

  if (mode == SET_MINUTE) {
    lcd.print(">");
    lcd.print(minute < 10 ? "0" : "");
    lcd.print(minute);
    lcd.print("<:");
  } else {
    lcd.print(minute < 10 ? "0" : "");
    lcd.print(minute);
    lcd.print(":");
  }

  lcd.print(second < 10 ? "0" : "");
  lcd.print(second);

  //lcd.print(isPM ? " PM" : " AM");

  // --- Print day name on second line ---
  lcd.setCursor(0, 0);
  const char* days[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
  lcd.print(days[currentTime.dayOfTheWeek()]);
  lcd.print(" ");
  lcd.print(currentTime.day() < 10 ? "0" : "");
  lcd.print(currentTime.day());
  lcd.print("/");
  lcd.print(currentTime.month() < 10 ? "0" : "");
  lcd.print(currentTime.month());
  lcd.print("/");
  lcd.print(currentTime.year() < 10 ? "0" : "");
  lcd.print(currentTime.year());
}
