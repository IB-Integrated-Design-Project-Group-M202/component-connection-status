/*line sensor analogue*/
#include <Adafruit_MotorShield.h>

bool accel = true, decel = false;

uint8_t centreSpeed=0; //average speed of left and right wheel for updatting the motors

//pin definition 
const int lsc_pin=A2,  lsl_pin=A3, lsr_pin=A4;


//sensor readouts
int lsc=0, lsl=0, lsr=0;
uint8_t lsc_mapped=0, lsl_mapped=0, lsr_mapped=0;

//floor and celling
int lsc_max=980, lsl_max=380, lsr_max=920;
int lsc_min=100, lsl_min=64, lsr_min=122;

//thresholds
const uint8_t lsc_threshold = 150;
const uint8_t lsl_threshold = 150;
const uint8_t lsr_threshold = 150;

//mode
bool on_line=false;
bool junction=false;

//PID parameters
const float Kp=0;
const float Ki=0;
const float Kd=0;

//PID variables
int P=0, I=0, D=0, last_P=0;

int speed_difference=0;
uint8_t left_motor_speed, right_motor_speed;



void update_linesensors(){
    lsc = analogRead(lsc_pin);
    lsl = analogRead(lsl_pin);
    lsr = analogRead(lsr_pin);
    lsc_mapped = map(lsc, lsc_min, lsc_max, 0, 255);
    lsl_mapped = map(lsl, lsl_min, lsl_max, 0, 255);
    lsr_mapped = map(lsr, lsr_min, lsr_max, 0, 255);
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
    if(lsc_mapped>=lsc_threshold){
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
    Serial.begin(9600);
    Serial.println("0\t0\t0\t255");


}



void loop(){

    update_linesensors();

    P=lsl_mapped-lsr_mapped;
    I=I+P;
    D=P-last_P;
    last_P=P;
    
    speed_difference=Kp*P+Ki*I+Kd*D;

    left_motor_speed=centreSpeed - max(speed_difference, 45);
    right_motor_speed=centreSpeed + max(speed_difference, 45);

    //Serial.println(String(lsc)+"\t"+String(lsl)+"\t"+String(lsr));
    Serial.println(String(lsc_mapped)+"\t"+String(lsl_mapped)+"\t"+String(lsr_mapped));
}
