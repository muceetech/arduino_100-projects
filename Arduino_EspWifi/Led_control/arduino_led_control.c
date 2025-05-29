
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

String inputString = "";      // A string to hold incoming data
bool ledState = false;        // Current state of the LED
const int ledPin = 4; 

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin, ledState);
  Serial.println("led control from ESP32");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  
}

void loop() { 
  
 while (mySerial.available()) {
    char inChar = (char)mySerial.read();

    if (inChar == '\n' || inChar == '\r') {
      //inputString.trim();  // Remove any stray \r or whitespace

      if (inputString == "led on") {
        digitalWrite(ledPin, HIGH);
        Serial.println("Received: led on - LED ON");
      }
       if (inputString == "led off") {
        digitalWrite(ledPin, LOW);
        Serial.println("Received: led off - LED OFF");
      }

      inputString = "";  // Clear for next message
    } else {
      inputString += inChar;
    }
  }
}
