/*
  Arduino LSM6DS3 - Simple Gyroscope

  This example reads the gyroscope values from the LSM6DS3
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - Arduino Uno WiFi Rev 2 or Arduino Nano 33 IoT

  created 10 Jul 2019
  by Riccardo Rizzo

  This example code is in the public domain.
*/

#include <Arduino_LSM6DS3.h>

int readings = 0;
float x, y, z, x_offset = 0, y_offset = 0, z_offset = 0, x_turn = 0, y_turn = 0, z_turn = 0, angle_turned = 0, x_total = 0, y_total = 0, z_total = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");

  

  while (readings < 10000) {
    if (IMU.gyroscopeAvailable()) IMU.readGyroscope(x, y, z);
    x_total += x; y_total += y; z_total += z; readings += 1;
    x_offset = x_total / readings; y_offset = y_total / readings; z_offset = z_total / readings;
  }
}

unsigned long currentMillis, lastMillis = 0, elapsedMillis;

void loop() {
  float x, y, z;

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);

    currentMillis = millis();
    elapsedMillis = currentMillis-lastMillis;

    x_turn += (x - x_offset)*elapsedMillis/1000;
    y_turn += (y - y_offset)*elapsedMillis/1000;
    z_turn += (z - z_offset)*elapsedMillis/1000;

    delay(0);

    lastMillis = currentMillis;
    
    Serial.print(x - x_offset);
    Serial.print('\t');
    Serial.print(y-y_offset);
    Serial.print('\t');
    Serial.println(z-z_offset);
    Serial.print('\t');
    Serial.print(x_turn);
    Serial.print('\t');
    Serial.println(y_turn);
    Serial.print('\t');
    Serial.println(z_turn);
  }
}
