#define PT1 A0
#define PT2 A1
#define IR 7

//sensor values
int sensorValue1, sensorValue2;
int receiverValue;
int dummy = 0;
//Calibration
unsigned long mil;
int sensorMax1 = 0, sensorMax2 = 0, sensorMin1 = 1023, sensorMin2 = 1023;
int readings_1 = 0;
int readings_2 = 0;
int true_readings_1 = 0;
int true_readings_2 = 0;

double mapf(double val, double in_min, double in_max, double out_min, double out_max) {
    return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup() {
  pinMode(PT1, INPUT);
  pinMode(PT2, INPUT);
  pinMode(IR, INPUT);
  Serial.begin(9600);
  
  mil = millis();
  if(sensorValue1 >= 500){delay(4);} //leave the first spike if there is one
  while(millis() - mil < 100){
    sensorValue1 = analogRead(PT1);
    sensorValue2 = analogRead(PT2);
    if(sensorValue1 > sensorMax1){sensorMax1 = sensorValue1;}
    if(sensorValue2 > sensorMax2){sensorMax2 = sensorValue2;}
    if(sensorValue1 < sensorMin1){sensorMin1 = sensorValue1;}
    if(sensorValue2 < sensorMin2){sensorMin2 = sensorValue2;}
  }
}

void loop() {
  
  /* Look at the point where signal from phototransistor is higher
   *  than 500 (beginning of a spike)and then checks whether IR sensor gets low for this
   *  spike.
  */

  mil = millis();
  int readings_1 = 0;
  int readings_2 = 0;
  int true_readings_1 = 0;
  int true_readings_2 = 0;
  while(millis() - mil < 650){
    
    sensorValue1 = analogRead(PT1);
    sensorValue1 = constrain(sensorValue1, sensorMin1, sensorMax1);
    sensorValue1 = map(sensorValue1, sensorMin1, sensorMax1, 0, 1023);
     
    if(sensorValue1 >= 200){
      readings_1++;
      delayMicroseconds(300);
      if(digitalRead(IR) == LOW){
        true_readings_1++;
      }
      delay(4);
    }
  }

  if(sensorValue2 >= 500){delay(4);} //leave the first spike if there is one

  mil = millis();
  while(millis() - mil < 650){
    
    sensorValue2 = analogRead(PT2);
    sensorValue2 = constrain(sensorValue2, sensorMin2, sensorMax2);
    sensorValue2 = map(sensorValue2, sensorMin2, sensorMax2, 0, 1023);
     
    if(sensorValue2 >= 200){
      readings_2++;
      delayMicroseconds(300);
      if(digitalRead(IR) == LOW){
        true_readings_2++;
      }
      delay(4);
    } 
  }

  float percentage_1 = float(float(true_readings_1) / float(readings_1));
  float percentage_2 = float(float(true_readings_2) / float(readings_2));
  short dummy_range_1 = mapf(percentage_1, 0.00, 1.00, 0, 2);
  short dummy_range_2 = mapf(percentage_2, 0.00, 1.00, 0, 2);
  if(dummy_range_1 == 2 or dummy_range_2 == 2){dummy = 1;}
  else if(dummy_range_1 == 0 or dummy_range_2 == 0){dummy = 2;}
  else if(dummy_range_1 == 1 or dummy_range_2 == 1){dummy = 3;}
  Serial.print(readings_1); Serial.print('\t');
  Serial.print(readings_2); Serial.print('\t');
  Serial.print(true_readings_1); Serial.print('\t');
  Serial.print(true_readings_2); Serial.print('\t');
  Serial.print(percentage_1); Serial.print('\t');
  Serial.print(percentage_2); Serial.print('\t');
  Serial.println(dummy);
}
