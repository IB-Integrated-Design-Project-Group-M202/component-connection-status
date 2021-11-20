
// Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.
const int IRPT1 = A0;
const int IRPT2 = A1;
const int IRRPin = 7;

int IRPT1Value;
int IRPT2Value;      // the readings from the analog input       
int IRRValue = 0;



void setup() {
    // initialize serial communication with computer:
    Serial.begin(19200);
    // initialize all the readings to 0:
    pinMode(IRRPin, INPUT);
}

void loop() {

  // calculate the average:
  //average = total / numReadings;
    IRPT1Value = analogRead(IRPT1);
    IRPT1Value = analogRead(IRPT1);
    IRRValue = digitalRead(IRRPin);
  // send it to the computer as ASCII digits
    Serial.print(IRPT1Value);
    Serial.print('\t');
    Serial.print(IRPT2Value);
    Serial.print('\t');
    Serial.println(IRRValue*1023);
    
    delay(1);        // delay in between reads for stability
}

