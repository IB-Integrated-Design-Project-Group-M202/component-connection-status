unsigned long previousMillis = 0;
long duration;
const int ledPin = 8, echoPin = 2, trigPin = 3;
int distance = 0, ledState = LOW;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  unsigned long currentMillis = millis();
  unsigned long time_elapsed = currentMillis - previousMillis;
  if ((time_elapsed <= 5) && (distance == 0)) {
    digitalWrite(trigPin, LOW);
    delay(2);
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 3.4 / 20;
    if (distance < 2500) Serial.println(distance);
  }
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
    distance = 0;
  }
}
