#define PT1 A0
#define PT2 A1
#define IR 7

//sensor values
int sensorValue1, sensorValue2;
int receiverValue;
int dummy = 0;

void setup() {
  pinMode(PT1, INPUT);
  pinMode(PT2, INPUT);
  pinMode(IR, INPUT);

}

void loop() {

  repeat:
  
  //Calibration
  int mil = millis();
  int sensorMax1 = 0, sensorMax2 = 0, sensorMin1 = 1023, sensorMin2 = 1023;
  while(millis() - mil < 100){
    sensorValue1 = analogRead(PT1);
    sensorValue2 = analogRead(PT2);
    if(sensorValue1 > sensorMax1){sensorMax1 = sensorValue1;}
    if(sensorValue2 > sensorMax2){sensorMax2 = sensorValue2;}
    if(sensorValue1 < sensorMin1){sensorMin1 = sensorValue1;}
    if(sensorValue2 < sensorMin2){sensorMin2 = sensorValue2;}
  }

  /* Look at the point where signal from phototransistor is higher
   *  than 500 (beginning of a spike)and then checks whether IR sensor gets low for this
   *  spike.
  */
  int readings_1 = 0;
  int true_readings_1 = 0;
  if(sensorValue1 >= 500){delay(4);} //leave the first spike if there is one

  mil = millis();
  while(millis() - mil < 2450){
    
    sensorValue1 = analogRead(PT1);
    sensorValue1 = constrain(sensorValue1, sensorMin1, sensorMax1);
    sensorValue1 = map(sensorValue1, sensorMin1, sensorMax1, 0, 1023);
     
    if(sensorValue1 >= 500){
      readings_1++;
      delayMicroseconds(300);
      if(digitalRead(IR) == LOW){
        true_readings_1++;
      }
      delay(4);
    }
  }

  int readings_2 = 0;
  int true_readings_2 = 0;
  if(sensorValue2 >= 500){delay(4);} //leave the first spike if there is one

  mil = millis();
  while(millis() - mil < 2450){
    
    sensorValue2 = analogRead(PT2);
    sensorValue2 = constrain(sensorValue2, sensorMin2, sensorMax2);
    sensorValue2 = map(sensorValue2, sensorMin2, sensorMax2, 0, 1023);
     
    if(sensorValue2 >= 500){
      readings_2++;
      delayMicroseconds(300);
      if(digitalRead(IR) == LOW){
        true_readings_2++;
      }
      delay(4);
    }
  }

  int percentage_1 = 100*true_readings_1/readings_1;
  int percentage_2 = 100*true_readings_2/readings_2;
  if(pulseIn(IR, LOW, 10000) < 500){dummy = 2;}
  else if(percentage_1 > 90 and percentage_2 > 90){dummy = 1;}
  else if(percentage_1 > 40 and percentage_2 > 40 and percentage_1 < 60 and percentage_2 < 60){dummy = 3;}
  else if(percentage_1 < 10 and percentage_2 < 10){dummy = 2;}
  else{goto repeat;}
  
}
