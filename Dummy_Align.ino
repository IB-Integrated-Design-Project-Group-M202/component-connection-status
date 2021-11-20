
// Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.
const int numReadings = 150;

int readings1[numReadings];
int readings2[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;
int maxValue1 = 0;
int maxIndex1 = 0;  
int maxValue2 = 0;
int maxIndex2 = 0;         
int IRRValue = 0;

const int IRPT1 = A0;
const int IRPT2 = A1;
const int IRRPin = 7;

void setup() {
    // initialize serial communication with computer:
    Serial.begin(19200);
    // initialize all the readings to 0:
    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
        readings1[thisReading] = 0;
        readings2[thisReading] = 0;
    }
    pinMode(IRRPin, INPUT);
}

void loop() {
    // subtract the last reading:
    //total = total - readings[readIndex];


    // read from the sensor:
    readings1[readIndex] = analogRead(IRPT1);
    readings2[readIndex] = analogRead(IRPT2);
    IRRValue = digitalRead(IRRPin);
    
    if (maxIndex1 == readIndex) {
        maxValue1 = 0;
        for(int i = 1; i < numReadings; i++)
        {
            if(readings1[i] > maxValue1) {
                maxValue1 = readings1[i];
                maxIndex1 = i;
            }
        }
    }

    if (maxIndex2 == readIndex) {
        maxValue2 = 0;
        for(int i = 1; i < numReadings; i++)
        {
            if(readings2[i] > maxValue2) {
                maxValue2 = readings2[i];
                maxIndex2 = i;
            }
        }
    }
    // add the reading to the total:
    //total = total + readings[readIndex];
    if(readings1[readIndex] > maxValue1) {
        maxValue1 = readings1[readIndex];
        maxIndex1 = readIndex;
        }
    if(readings2[readIndex] > maxValue2) {
        maxValue2 = readings2[readIndex];
        maxIndex2 = readIndex;
        }
    // advance to the next position in the array:
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= numReadings) {
        // ...wrap around to the beginning:
        readIndex = 0;
    }

  // calculate the average:
  //average = total / numReadings;

  // send it to the computer as ASCII digits
    Serial.print(IRRValue);
    Serial.print('\t');
    Serial.print(maxValue1);
    Serial.print('\t');
    Serial.print(maxValue2);
    Serial.print('\t');
    Serial.print(readings1[readIndex]);
    Serial.print('\t');
    Serial.println(readings2[readIndex]);
    
    delay(1);        // delay in between reads for stability
}

