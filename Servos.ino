#include <Servo.h>

#define ServoPIN 9
Servo myservo;

int val;

void setup() {
  myservo.attach(ServoPIN);

}

void loop() {
  val = 30;
  myservo.write(val);
  delay(1000);
  val = 120;
}
