#include <Adafruit_MotorShield.h>
#include <Arduino_LSM6DS3.h>
#include <HCSR04.h>

bool accel = true, decel = false;


//distance sensors
#define echoPin 2
#define trigPin 3
double distance;

uint8_t ranging_index=0;

// Global variables and definitions for motors
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); // Create the motor shield object with the default I2C address
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1); // Select and configure port M1
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2); // Select and configure port M2
uint8_t centreSpeed = 0, leftSpeed = 0, rightSpeed = 0, leftDirection = FORWARD, rightDirection = FORWARD;

int speed_difference = 0;

int leftSpeedv, rightSpeedv;

//mode
bool in_range=false;
//bool locked=false;
bool stop=false;
bool sequence_timeout=false;
//int in_range_indication=0;

unsigned long search_timer=0;
unsigned long now_ms=0;


//parameters
const float Kp=0.04;
const float Ki=0.001;
const float Kd=0.1;
const int in_range_threashold=750;
const uint8_t approachSpeed=170;
const uint8_t turnSpeed=170;
const unsigned long search_timeout=10000;

//PID variables
long P=0, I=0, D=0, last_P=0;
float gapf=0;
/*

Naming conventions:

S1 - Sensor 1
m1 - Maximum readout sequence 1

d - Detect mode
s - Stored value


t1 - Time Stamp 1
tm1 - Timer 1

(1 for detection window, 2 for hold)
*/

//sensor interface
const int IRs1 = A0, IRs2 = A1, IRRd=7;
//time parameters
const unsigned long window_time=3000, hold_time=12500;
const int a_size=20;


int s1m1sa[a_size], s2m1sa[a_size];
int a_i=0;
int s1m1sat=0, s1m1saa=0, s2m1sat=0, s2m1saa=0;
int sdiff=0, ssum=0;
//time variables
unsigned long s1m1tm1=0, s1m1tm2=0, s1m1t1=0, s1m1t2=0, s2m1tm1=0, s2m1tm2=0, s2m1t1=0, s2m1t2=0, lastPID=0, gap=0, now=0;

//sensor values
int s1 = 0, s2 =0;
//processed sensor values
int s1m1=0, s1m1s=0, s2m1=0, s2m1s=0;
//loop phase
bool s1m1d = false;
//output
String output="";

double measure_distance_mm() {
    double* distances = HCSR04.measureDistanceMm();
    return distances[0];
}

void if_stop(){

    if(ranging_index%16==0){
        distance=measure_distance_mm();
        if(distance<=180 and distance!=-1){
            stop=true;
        }
    }
    ranging_index++;
}

void reset_PID(){
    P=0;
    I=0;
    D=0;
    last_P=0;
}

void IR_readout(){
    s1 = analogRead(IRs1);
    s2 = analogRead(IRs2);
    now = micros();
}

void IR_peak_update(){
    if(s1 > s1m1){
            s1m1 = s1;
            s2m1 = s2;
            s1m1t1 = now;
            if(s1m1>s1m1s){
                s1m1s=s1m1;
                s2m1s=s2m1;
                s1m1t2=now;
            }
    }
}

void if_in_range(){
    //in_range_indication=abs((s1m1saa-s1)*ssum / 100);
    if(ssum>=in_range_threashold){
        if(in_range==false){
            in_range=true;
            reset_PID();
            centreSpeed=approachSpeed;
        }
    }
    else{
        in_range=false;
        centreSpeed=0;
    }
}

void PID_update(){
    gap=now-lastPID;
    gapf=gap/5000;
    if(in_range==true){
        P=sdiff;
        I=I+P*gapf/10;
        D=(P-last_P)/gapf;
        last_P=P;
        speed_difference=max(min((Kp*P+Ki*I+Kd*D), turnSpeed), -turnSpeed);
        if_stop();
    }
    else{
        speed_difference=turnSpeed;
    }
    lastPID=now;
    leftSpeedv=centreSpeed-speed_difference;
    rightSpeedv=centreSpeed+speed_difference;
    leftSpeed=abs(leftSpeedv);
    rightSpeed=abs(rightSpeedv);

}

void motor_update(){
    if(leftSpeedv>0){
        leftDirection=FORWARD;
    }
    else{
        leftDirection=BACKWARD;
    }
    if(rightSpeedv>0){
        rightDirection=FORWARD;
    }
    else{
        rightDirection=BACKWARD;
    }
    leftMotor->setSpeed(leftSpeed);
    rightMotor->setSpeed(rightSpeed);
    leftMotor->run(leftDirection);
    rightMotor->run(rightDirection);
}

void setup() {
    //Serial.begin(9600);
    for (int i = 0; i < a_size; i++){
        s1m1sa[i]=0;
        s2m1sa[i]=0;
    }
    //Serial.println("0\t0\t0\t0\t-1023\t1023");
  if (!AFMS.begin()) { // Check whether the motor shield is properly connected
    while (1);
  }
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
    HCSR04.begin(trigPin, echoPin);
  digitalWrite(trigPin, LOW);
}


void search(){

    //Serial.println("loop"+String(sequence_timeout)+String(distance)+String(ranging_index));
    search_timer=millis();
    sequence_timeout=false;
    while(stop==false and sequence_timeout==false){
        IR_readout();

        IR_peak_update();

        s1m1tm1=now-s1m1t1;

        //timeout
        if(s1m1tm1>=window_time){
            //initialises hold
            s1m1d=false;
            s1m1=0;
            s2m1=0;

            s1m1sat=s1m1sat-s1m1sa[a_i];
            s2m1sat=s2m1sat-s2m1sa[a_i];
            s1m1sat+=s1m1s;
            s2m1sat+=s2m1s;
            s1m1sa[a_i]=s1m1s;
            s2m1sa[a_i]=s2m1s;
            a_i+=1;
            if (a_i>=a_size){
                a_i=0;
            }
            s1m1saa=s1m1sat/a_size;
            s2m1saa=s2m1sat/a_size;
            ssum=s1m1saa+s2m1saa;
            sdiff=s1m1saa-s2m1saa;
            if_in_range();

            PID_update();
            motor_update();
            if(((now_ms-search_timer)>=search_timeout & in_range==false)){
              sequence_timeout=true;
            }
            //Serial.println("search"+String(distance));
            //Serial.print(String(s1m1saa)+"\t"+String(s2m1saa)+'\t');
            //Serial.print(String(ssum)+"\t"+String(sdiff)+'\t');
            //Serial.println(String(P)+"\t"+String(I/1000)+"\t"+String(D)+'\t');
            //Serial.println(String(speed_difference)+"\t"+String(in_range*1024));
           // Serial.println(String(s1m1saa)+"\t"+String(s2m1saa)+'\t'+String(ssum)+"\t"+String(sdiff)+'\t'+String(P)+"\t"+String(I/1000)+"\t"+String(D)+'\t');

            //Serial.println(String(s1m1saa)+'\t'+String(s2m1saa)+'\t'+String(in_range*1023)+'\t'+String(speed_difference));
        }

        s1m1tm2=now-s1m1t2;
            //timeout
        if(s1m1tm2>=hold_time){
                //initiates detection window
            s1m1d=true;
            s1m1t2=now;
            s1m1s=s1m1;
            s2m1s=s2m1;
        }

        now_ms=millis();
    }
}

void walk(){
    search_timer=millis();
    sequence_timeout=false;
    while(stop==false & sequence_timeout==false){
        if_stop();
        //Serial.println("walk"+String(distance));
        leftSpeedv=245;
        rightSpeedv=220;
        motor_update();
        now_ms=millis();
        if(((now_ms-search_timer)>=3000)){
            sequence_timeout=true;
        }

    }
    
    
}

void escape(){
    stop=false;
    leftSpeedv=-250;
    rightSpeedv=150;
    leftSpeed=abs(leftSpeedv);
    rightSpeed=abs(rightSpeedv);
    motor_update();
    delay(2000);
}

void search_and_align_and_identify(){

    search();
    walk();

    if_stop();
    if(stop==true){
        leftSpeed=0;
        rightSpeed=0;
        motor_update();
    }
    
    if_in_range();

    if(in_range=true){
        delay(1000);
        //put identification here and delete delay
    }

    escape();

}


void loop() {
    search_and_align_and_identify();
}  