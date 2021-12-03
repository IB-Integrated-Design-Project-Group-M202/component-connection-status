#include <HCSR04.h>



#define echoPin 2
#define trigPin 3
double* distances;
unsigned short distance;

long start=0, fin=0;

double measure_distance_mm() {
    double* distances = HCSR04.measureDistanceMm();
    return distances[0];
}

void setup(){
    Serial.begin(9600);
}

void loop(){
    start=micros();
    measure_distance_mm();
    fin=micros();
    Serial.println((fin-start));
}
