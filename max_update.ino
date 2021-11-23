unsigned long s1m1tm1=0, s1m1tm2=0, s1m1t1=0, s1m1t2=0, lastt=0, gap=0, now=0;
int s1 = 0;
int s1m1=0;
bool s1m1d = false;
const unsigned long window_time=60000, hold_time=50000;
const int IRs1 = A0;

void setup() {
    Serial.begin(2000000);
}

void loop() {
    s1 = analogRead(IRs1);
    now = micros();
    if(s1m1d==false){
        s1m1tm2=now-s1m1t2;
        if(s1m1tm2>=hold_time){
            s1m1d=true;
            s1m1t1=now;
            s1m1=s1;
           // Serial.println("detect");
        }
    }

    if(s1m1d==true){
        s1m1tm1=now-s1m1t1;
        if(s1m1tm1>=window_time){
            //Serial.println("hold");
            s1m1d=false;
            s1m1t2=now;
        }
        if(s1 > s1m1){
            s1m1 = s1;
            s1m1t1 = now;
        }
    }





    gap=now-lastt;
    /*Serial.print(s1m1tm1);
    Serial.print('\t');
    Serial.print(s1m1tm2);
    Serial.print('\t');
    Serial.print(s1m1d);
    Serial.print('\t');
    Serial.print(s1);
    Serial.print('\t');
    Serial.print(s1m1);
    Serial.print('\t');*/   
    Serial.println(gap);
    lastt=now;
} 