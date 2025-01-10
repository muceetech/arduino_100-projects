// Simple Traffic Light interface using Muceeshield

int RED_led = 12;
int AMBER_led = 11;
int GREEN_led = 10;

void setup() {
pinMode(RED_led, OUTPUT);
pinMode(AMBER_led, OUTPUT);
pinMode(GREEN_led, OUTPUT);
}

void loop() {
digitalWrite(RED_led, HIGH);
digitalWrite(AMBER_led, LOW);
digitalWrite(GREEN_led, LOW);
delay(3000);

digitalWrite(RED_led, LOW);
digitalWrite(AMBER_led, HIGH);
digitalWrite(GREEN_led, LOW);
delay(1500);

digitalWrite(RED_led, LOW);
digitalWrite(AMBER_led, LOW);
digitalWrite(GREEN_led, HIGH);
delay(3000);


}



