#include <hidboot.h>
#include <usbhub.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
int line = 0;
uint8_t cursorCol = 0;
uint8_t cursorRow = 0;

USB     Usb;
HIDBoot<USB_HID_PROTOCOL_KEYBOARD> Keyboard(&Usb);

class KbdRptParser : public KeyboardReportParser {
  void OnKeyDown(uint8_t mod, uint8_t key) {
    uint8_t c = OemToAscii(mod, key);
    if (c) 
    {
      Serial.print((char)c);
      lcd.cursor();
      lcd.setCursor(cursorCol, cursorRow);
      lcd.print((char)c);
      cursorCol++;
      if (cursorCol >= 16) {
        cursorCol = 0;
        cursorRow = (cursorRow + 1) % 2; // Assuming 2-line LCD
    } 
    }
    if (key == 0x2A) {  // Backspace
        if (cursorCol > 0) {
          cursorCol--;
        } else if (cursorRow > 0) {
          cursorRow--;
          cursorCol = 15;
        } 
        lcd.setCursor(cursorCol, cursorRow);
        lcd.print(' ');
        lcd.setCursor(cursorCol, cursorRow);
     }
    /*
    if(key==0x28)
    {
      if(cursorRow==0) cursorRow=1; 
      else if (cursorRow==1)
      {
        cursorRow=0;
        lcd.clear();
      } 
      lcd.setCursor(0,cursorRow);
    }*/
    if (key == 0x28) {  // Enter key
         if (cursorRow == 0) {
          // Clear only second row
          lcd.setCursor(0, 1);
          for (int i = 0; i < 16; i++) {
            lcd.print(' ');
          }
          lcd.setCursor(0, 1);
          cursorCol = 0;
          cursorRow = 1;
        } else {
          // Clear entire screen
          lcd.clear();
          cursorCol = 0;
          cursorRow = 0;
        }
    }

  }
};

KbdRptParser Prs;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting USB Keyboard Input");
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("USB Host Example");
  lcd.setCursor(0,1);
  lcd.print("Keyboard demo");

  if (Usb.Init() == -1) {
    Serial.println("USB host shield init failed");
    while (1);
  }
  delay(200);
  Keyboard.SetReportParser(0, &Prs);
}

void loop() {
  Usb.Task();
}
