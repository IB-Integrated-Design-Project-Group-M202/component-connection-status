#include <HCSR04.h>

unsigned long previousMillis = 0;
long duration;
const int ledPin = 8, echoPin = 2, trigPin = 3;
int distance = 0, ledState = LOW;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  HCSR04.begin(trigPin, echoPin);
}

// the loop function runs over and over again forever
void loop() {
  unsigned long currentMillis = millis();
  unsigned long time_elapsed = currentMillis - previousMillis;
  if (time_elapsed >= 250) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
    double* distances = HCSR04.measureDistanceMm();
    if (distances[0] < 150) delay(5000);
  }
}
