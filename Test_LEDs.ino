const int greenLED_Pin = 12, amberLED_Pin = 8, redLED_Pin = 13;

void setup() {
  
  // Configure LEDs
  pinMode(amberLED_Pin, OUTPUT);
  pinMode(redLED_Pin, OUTPUT);
  pinMode(greenLED_Pin, OUTPUT);
  digitalWrite(amberLED_Pin, LOW);
  digitalWrite(redLED_Pin, LOW);
  digitalWrite(greenLED_Pin, LOW);
  
}

void loop() {
  
  // Flash all 3 LEDs at a frequency of about 2 Hertz
  digitalWrite(amberLED_Pin, HIGH);
  digitalWrite(redLED_Pin, HIGH);
  digitalWrite(greenLED_Pin, HIGH);
  delay(250);
  digitalWrite(amberLED_Pin, LOW);
  digitalWrite(redLED_Pin, LOW);
  digitalWrite(greenLED_Pin, LOW);
  delay(250);
  
}
