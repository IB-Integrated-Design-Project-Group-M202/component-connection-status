/* Tomasz Twardoch Nov 2021*/

#include <Arduino_LSM6DS3.h>

int readings = 0;
float x, y, angle, angle_offset = 0, angle_turned = 0, angle_total = 0;

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

  

  while (readings < 20000) {
    if (IMU.gyroscopeAvailable()) IMU.readGyroscope(x, y, angle);
    angle_total += angle; readings += 1;
    angle_offset = angle_total / readings;
  }
}

unsigned long currentMicros, lastMicros = 0, elapsedMicros;

void loop() {
  float x, y, angle;

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, angle);

    currentMicros = micros();
    elapsedMicros = currentMicros-lastMicros;

    angle_turned += (angle - angle_offset)*elapsedMicros/1000*180/160.7/1000;

    delay(50);

    lastMicros = currentMicros;
    
    Serial.println(angle-angle_offset);
    Serial.print('\t');
    Serial.println(angle_turned);
    Serial.print('\t');
    Serial.println(elapsedMicros);
    
  }
}
