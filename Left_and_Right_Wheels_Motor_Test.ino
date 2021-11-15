/* Pragash Mohanarajah (C) November 2021. */

/*
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2
---->	http://www.adafruit.com/products/1438
*/

#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select and configure port M1
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
// Select and configure port M2
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motor Shield v2.3 Test for DC Motors M1 & M2");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set a speed of 150 to start, where 0 is off & 255 is max speed
  leftMotor->setSpeed(150);
  rightMotor->setSpeed(150);
  // Turn on motors
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  // Turn off motors
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
}

void loop() {
  uint8_t i;

  Serial.println("Forward Drive");
  // Configure both motors to run forwards
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  // Toggle speed to reach maximum from zero
  for (i=0; i<255; i++) {
    leftMotor->setSpeed(i);
    rightMotor->setSpeed(i);
    delay(10);
  }
  // Toggle speed to reach zero from maximum
  for (i=255; i!=0; i--) {
    leftMotor->setSpeed(i);
    rightMotor->setSpeed(i);
    delay(10);
  }

  Serial.println("Backward Drive");
  // Configure both motors to run backwards
  leftMotor->run(BACKWARD);
  rightMotor->run(BACKWARD);
  // Toggle speed to reach maximum from zero
  for (i=0; i<255; i++) {
    leftMotor->setSpeed(i);
    rightMotor->setSpeed(i);
    delay(10);
  }
  // Toggle speed to reach minimum from zero
  for (i=255; i!=0; i--) {
    leftMotor->setSpeed(i);
    rightMotor->setSpeed(i);
    delay(10);
  }
 
  Serial.println("End Drive Test");
  // Finish drive test by leaving motors on standby
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
  delay(1000);

  Serial.println("Left Turn");
  // Configure robot to turn left, i.e. anti-clockwise
  // Configure left motor to run backwards
  leftMotor->run(BACKWARD);
  // Configure right motor to run forwards
  rightMotor->run(FORWARD);
  // Toggle speed to reach maximum from zero
  for (i=0; i<255; i++) {
    leftMotor->setSpeed(i);
    rightMotor->setSpeed(i);
    delay(10);
  }
  // Toggle speed to reach zero from maximum
  for (i=255; i!=0; i--) {
    leftMotor->setSpeed(i);
    rightMotor->setSpeed(i);
    delay(10);
  }

  Serial.println("Right Turn");
  // Configure robot to turn right, i.e. clockwise
  // Configure left motor to run forwards
  leftMotor->run(FORWARD);
  // Configure right motor to run backwards
  rightMotor->run(BACKWARD);
  // Toggle speed to reach maximum from zero
  for (i=0; i<255; i++) {
    leftMotor->setSpeed(i);
    rightMotor->setSpeed(i);
    delay(10);
  }
  // Toggle speed to reach zero from maximum
  for (i=255; i!=0; i--) {
    leftMotor->setSpeed(i);
    rightMotor->setSpeed(i);
    delay(10);
  }

  Serial.println("End Turn Test");
  // Finish turn test by leaving motors on standby
  leftMotor->run(RELEASE);
  rightMotor->run(RELEASE);
  delay(1000);
}
