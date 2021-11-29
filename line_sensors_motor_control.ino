/*line sensor analogue*/
#include <Adafruit_MotorShield.h>

bool accel = true, decel = false;

//pin definition 
#define lsc_pin 5
#define lsl_pin A3
#define lsr_pin A4

// Global variables and definitions for motors
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); // Create the motor shield object with the default I2C address
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1); // Select and configure port M1
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2); // Select and configure port M2
uint8_t centreSpeed = 0, leftSpeed = 0, rightSpeed = 0, speed_difference = 0, leftDirection = FORWARD, rightDirection = FORWARD;

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

//PID parameters
const float Kp=0.2;
const float Ki=0.01;
const float Kd=0.01;

//PID variables
int P=0, I=0, D=0, last_P=0;

void update_linesensors(){
    lsc = digitalRead(lsc_pin);
    lsl = analogRead(lsl_pin);
    lsr = analogRead(lsr_pin);
    lsl_mapped = map(min(max(lsl, lsl_min), lsl_max), lsl_min, lsl_max, 0, 255);
    lsr_mapped = map(min(max(lsr, lsr_min), lsr_max), lsr_min, lsr_max, 0, 255);
}

/*void callibrate(){
}*/


void reset_PID(){
    P=0;
    I=0;
    D=0;
    last_P=0;
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

void setup(){
  pinMode(lsc_pin, INPUT);
  pinMode(lsl_pin, INPUT);
  pinMode(lsr_pin, INPUT);
}



void loop(){

    update_linesensors();

    P=lsl_mapped-lsr_mapped;
    I=I+P;
    D=P-last_P;
    last_P=P;
    
    speed_difference=max(min((Kp*P+Ki*I+Kd*D), 45), -45);
    leftSpeed=centreSpeed - speed_difference;
    rightSpeed=centreSpeed + speed_difference;
    
    leftMotor->run(leftDirection);
    rightMotor->run(rightDirection);
    leftMotor->setSpeed(leftSpeed);
    rightMotor->setSpeed(rightSpeed);
}
