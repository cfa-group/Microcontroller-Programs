

//L298N
int in1 = 4;
int in2 = 5;
int in3 = 6;
int in4 = 7;


//PIR 
const int SPir = 2;
int sttsPir = 0;

//HC-SR04
#define echoPin 12 //Echo Pin
#define trigPin 11 //Trigger Pin

int maximumRange = 200; //kebutuhan akan maksimal range
int minimumRange = 00; //kebutuhan akan minimal range
long duration, distance; //waktu untuk kalkulasi jarak

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 pinMode(in1, OUTPUT);
 pinMode(in2, OUTPUT);
 pinMode(in3, OUTPUT);
 pinMode(in4, OUTPUT);

 pinMode(SPir, INPUT);

 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 
}

void loop() {
  // put your main code here, to run repeatedly:

}


uint8_t jarak(){
  digitalWrite(trigPin, LOW);delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration/58.2;
  Serial.println(distance);Serial.println(" Cm");
  return distance;
  }


uint8_t pir(){
  sttsPir = digitalRead(SPir);
  if (sttsPir == HIGH){
    Serial.println("Stat PIR: 1");
    }else{
      Serial.println("Stat PIR: 0");
      }
    return sttsPir;
  }


void forward(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  }

void backof(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  }
  
void left(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  }

void right(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  }  
