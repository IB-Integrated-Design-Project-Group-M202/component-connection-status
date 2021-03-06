// Define Electrical parameters
const int leftLineSensor = A2, rightLineSensor = A3;

void setup() {
  // Configure Line Sensors as Inputs
  pinMode(leftLineSensor, INPUT);
  pinMode(rightLineSensor, INPUT);
  // Begin Serial connection at 9600 bps
  Serial.begin(9600);
}

void loop() {
  // Continuously monitor the state of each line sensor.
  // Tell Serial to turn Left and Right according to which sensor is high.
  if (digitalRead(leftLineSensor) == HIGH && digitalRead(rightLineSensor) == HIGH) Serial.println(F("Crossroad"));
  else if (digitalRead(leftLineSensor) == HIGH) Serial.println(F("Turn Left"));
  else if (digitalRead(rightLineSensor) == HIGH) Serial.println(F("Turn Right"));
}
