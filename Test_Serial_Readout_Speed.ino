uint32_t added = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);

}

void loop() {
  // put your main code here, to run repeatedly:

  uint32_t now;

  added = added + 100;
  now = micros();
  Serial.print(millis());
  Serial.print(" ");
  now = micros() - now;
  Serial.print(now);
  Serial.print('\t');
  Serial.println(added);
  

  delay(100);
}
