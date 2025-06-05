const int txPin = A2;
const int btnPins[] = {2, 3, 4, 5};
const unsigned int delays[] = {500, 1000, 1500, 2000}; // microseconds for each button

void setup() {
  pinMode(txPin, OUTPUT);
  digitalWrite(txPin, LOW);

  for (int i = 0; i < 4; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
  }
}

void sendPulse(unsigned int duration) {
  digitalWrite(txPin, HIGH);
  delayMicroseconds(duration);
  digitalWrite(txPin, LOW);
  delay(100); // Give time before next press
}

void loop() {
  for (int i = 0; i < 4; i++) {
    if (digitalRead(btnPins[i]) == LOW) {
      sendPulse(delays[i]);
      while (digitalRead(btnPins[i]) == LOW); // Debounce: wait for release
    }
  }
}
