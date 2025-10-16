//Include All Library
#include <Wire.h>
#include <Servo.h>
#include <SPI.h>
  


//Var control driver Motor DC
const int in[] = {9, 6, A0, A3};

//Var Servos
Servo servoX; //Arm-X
Servo servoY; //Arm-Y
int pos = 0;

//Var HCSR-04
#define trig 10
#define echo 3
int maxi = 200;
int mini = 0;
long duration, dist;
int distR = 0;
int distL = 0;

//Var Flame Sensor 5 Channel
const int sch[] = {A1, A2, A4, A5};
int vSch[]= {00, 00, 00, 00} ;

//Var Relay
#define relay 2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  servoX.attach(4);
  servoY.attach(5);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  
  for(byte i=0;i<4;i++){
    pinMode(in[i], OUTPUT);
    }
    
  servoX.write(100);//servo Bawah
  servoY.write(70);//Servo Atas

   }


  

void loop() {
  
    vsc();

delay(300);

}



    

void forward(){
  digitalWrite(in[0], LOW);
  digitalWrite(in[1], HIGH);
  digitalWrite(in[2], LOW);
  digitalWrite(in[3], HIGH);
  

  }

void backof(){
  digitalWrite(in[0], HIGH);
  digitalWrite(in[1], LOW);
  digitalWrite(in[2], HIGH);     //Mundur func.
  digitalWrite(in[3], LOW);
  

  }
  
void left(){
  digitalWrite(in[0], HIGH);
  digitalWrite(in[1], LOW);
  digitalWrite(in[2], LOW);
  digitalWrite(in[3], HIGH);
  
  }


//-- balik kanan
void right(){
  digitalWrite(in[0], LOW);
  digitalWrite(in[1], HIGH);
  digitalWrite(in[2], HIGH);
  digitalWrite(in[3], LOW);
  
  }  

void stp(){
  digitalWrite(in[0], LOW);
  digitalWrite(in[1], LOW);
  digitalWrite(in[2], LOW);
  digitalWrite(in[3], LOW);
  
  }

    


uint8_t vsc(){
 for(byte i=0; i<4; i++){
    vSch[i] = analogRead(sch[i]);
    Serial.print("valSCh ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(vSch[i]);
  
    if(vSch[i] > 550){
      delay(100);
      blackout(50, 3); 
        }else{
          hc();
          Serial.print("Dist : ");
          Serial.println(dist);
          }
  }  
  
}

    

int blackout(int p, int d){
  stp();
  delay(500);
  wtr_pump(HIGH);
  delay(500);
  for(pos = 0; pos < 180; pos+=d){
    servoX.write(pos);
    servoY.write(pos);
    delay(p);  
    }
  for(pos = 180; pos > 0; pos-=d){
      servoY.write(pos);
      servoX.write(pos);
      delay(p);
    }
  for(pos = 0; pos < 180; pos+=d){
    servoX.write(pos);
    servoY.write(pos);
    delay(p);  
    }
  for(pos = 180; pos > 0; pos-=d){
      servoY.write(pos);
      servoX.write(pos);
      delay(p);
    }


    wtr_pump(LOW);
    servoX.write(100);
    servoY.write(70);

    delay (2500);
//    vsc();
}



uint8_t hc(){ // check dist...
  servoX.write(100);
  servoY.write(70);
  digitalWrite(trig, LOW);delayMicroseconds(2);
  digitalWrite(trig, HIGH);delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  dist = duration/58.2;
  
  if(dist <= 50){
    right();
    return dist;

    }else if(dist <= 40){
      left();
      return dist;
     
      }else{
        forward();
        return dist;

        }
     
  
}



uint8_t wtr_pump(int p){
  digitalWrite(relay, p);
  
  }
