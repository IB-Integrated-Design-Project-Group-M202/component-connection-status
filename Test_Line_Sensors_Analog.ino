//pin definition 
const int lsc_pin=5,  lsl_pin=A2, lsr_pin=A3;

//sensor readouts
int lsc=0, lsl=0, lsr=0;
uint8_t lsl_mapped=0, lsr_mapped=0;

//floor and celling
int lsl_max=380, lsr_max=920;
int lsl_min=64, lsr_min=122;

//thresholds
const uint8_t lsl_threshold = 150;
const uint8_t lsr_threshold = 150;

//mode
bool on_line=false;
bool junction=false;

void update_linesensors(){
    lsc = digitalRead(lsc_pin);
    lsl = analogRead(lsl_pin);
    lsr = analogRead(lsr_pin);
    lsl_mapped = map(min(max(lsl, lsl_min), lsl_max), lsl_min, lsl_max, 0, 255);
    lsr_mapped = map(min(max(lsr, lsr_min), lsr_max), lsr_min, lsr_max, 0, 255);
}

void if_on_line(){
    update_linesensors();
    if(lsc==1){
        if(on_line==false){
            on_line=true;
            reset_PID();
        }
    }
    else{
        on_line=false;
    }
}

void if_junction(){
    update_linesensors();
    if(on_line==true && lsr_mapped>=lsr_threshold && lsr_mapped>=lsr_threshold){
        junction=true;
    }
    else{
        junction=false;
    }
}

void setup() {
  // Configure Line Sensors as Inputs
  pinMode(leftLineSensor, INPUT);
  pinMode(centralLineSensor, INPUT);
  pinMode(rightLineSensor, INPUT);
  // Begin Serial connection at 9600 bps
  Serial.begin(9600);
}

void loop() {
  update_linesensors();
  if_on_line();
  if_junction();
  Serial.print(lsl_mapped);
  Serial.print('\t');
  Serial.print(lsc*1023);
  Serial.print('\t');
  Serial.print(lsr_mapped);
  Serial.print('\t');
  Serial.print(on_line*1023);
  Serial.print('\t');
  Serial.println(junction*1023);
}
