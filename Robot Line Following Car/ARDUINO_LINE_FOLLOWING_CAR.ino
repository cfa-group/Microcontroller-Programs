//ARDUINO LINE FOLLOWING CAR//
// YOU HAVE TO INSTALL THE AFMOTOR LIBRARY BEFORE UPLOAD THE CODE//
// GO TO SKETCH >> INCLUDE LIBRARY >> ADD .ZIP LIBRARY >> SELECT AF MOTOR ZIP FILE //
 
//including the libraries
#include <AFMotor.h>

//defining pins and variables
#define left A0
#define right A1
#define p1 A2
#define p2 A3
//defining motors
AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

int l;
int r;


void setup() {
  //declaring pin types
  pinMode(left,INPUT);
  pinMode(right,INPUT);
  pinMode(p1,OUTPUT);
  pinMode(p2,OUTPUT);
  digitalWrite(p1, LOW);
  digitalWrite(p1, LOW);
  delay(5000);
  //begin serial communication
  digitalWrite(p1, HIGH);
  digitalWrite(p1, HIGH);
  Serial.begin(9600);
  delay(5000);
}

void loop(){
  //printing values of the sensors to the serial monitor
  int l = digitalRead(left);
  int r = digitalRead(right);
    Serial.print(l);
    Serial.print(":::");
    Serial.println(r);
    delay(250);
  if(l == 1 && r == 1){
    Serial.println("maju");
    //Forward
    D();
  }//line detected by left sensor
    else if(l == 0 && r == 1){
      Serial.println("belok kiri");
      //turn left
      L();
    }//line detected by right sensor
      else if(l == 1 && r == 0){
        Serial.println("belok kanan");
        //turn right
        R();
      }//line detected by none
        else if(l == 0 && r == 0){
          Serial.println("stop!");
          //stop
          N();
        }
}

void D(){
    motor1.setSpeed(255);
    motor1.run(FORWARD);
    motor2.setSpeed(255);
    motor2.run(FORWARD);
    motor3.setSpeed(255);
    motor3.run(FORWARD);
    motor4.setSpeed(255);
    motor4.run(FORWARD);
  }

void N(){
    motor1.setSpeed(0);
    motor1.run(RELEASE);
    motor2.setSpeed(0);
    motor2.run(RELEASE);
    motor3.setSpeed(0);
    motor3.run(RELEASE);
    motor4.setSpeed(0);
    motor4.run(RELEASE);
  }

void R(){
    motor1.setSpeed(255);
    motor1.run(BACKWARD);
    motor2.setSpeed(255);
    motor2.run(BACKWARD);
    motor3.setSpeed(255);
    motor3.run(FORWARD);
    motor4.setSpeed(255);
    motor4.run(FORWARD);
  }

void L(){
    motor1.setSpeed(255);
    motor1.run(FORWARD);
    motor2.setSpeed(255);
    motor2.run(FORWARD);
    motor3.setSpeed(255);
    motor3.run(BACKWARD);
    motor4.setSpeed(255);
    motor4.run(BACKWARD);
  }
