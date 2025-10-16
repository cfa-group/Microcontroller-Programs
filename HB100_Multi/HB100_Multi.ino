
#define trig1  5
#define echo1  6
#define trig2  7
#define echo2  9

long duration1;
int distance1;
long duration2;
int distance2;


#define buzz 3
#define relay 4
#define Prelay 11

#define pir0  8
int Dpir0 = 0;
#define pir1  2
int Dpir1 = 0;

void setup() {

  pinMode(Prelay, OUTPUT);
  digitalWrite(Prelay, HIGH);
  pinMode(relay, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(trig1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT);
  pinMode(echo2, INPUT);
  digitalWrite(buzz, LOW);
  digitalWrite(relay, LOW);
  pinMode(pir0, INPUT);
  pinMode(pir1, INPUT);
  Serial.begin(57600);
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(10);
//  FreqMeasure.begin();
  Serial.println("FreqMeasureMulti Begin");
//  digitalWrite(relay, HIGH);
  delay(100);


}



void loop() {


  Dpir0 = digitalRead(pir0);
  Dpir1 = digitalRead(pir1);
  Serial.print("D_Pir : ");
  Serial.println(Dpir0);
  Serial.println();
  
  Serial.print("D_Pir : ");
  Serial.println(Dpir1);
  Serial.println();
  if (Dpir0 == 0 || Dpir1 == 0){
  digitalWrite(buzz, HIGH);
  digitalWrite(relay, HIGH);
  delay (10);
  distance1 = GetDist1();
  Serial.print("Dist1 :   ");
  Serial.println(distance1);
  Serial.println("");
  
  distance2 = GetDist2();
  Serial.print("Dist2 :   ");
  Serial.println(distance2);
  Serial.println("");
  }else{
    digitalWrite(buzz, LOW);
    digitalWrite(relay, LOW);
    }
  
delay(70);   
}




int GetDist1(){
   digitalWrite(trig1, LOW); 
   delayMicroseconds(2);
   // Sets the trigPin on HIGH state for 10 micro seconds
   digitalWrite(trig1, HIGH); 
   delayMicroseconds(10);
   digitalWrite(trig1, LOW);
   duration1 = pulseIn(echo1, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
   distance1= duration1*0.034/2;
   return distance1;

  }



int GetDist2(){
   digitalWrite(trig2, LOW); 
   delayMicroseconds(2);
   // Sets the trigPin on HIGH state for 10 micro seconds
   digitalWrite(trig2, HIGH); 
   delayMicroseconds(10);
   digitalWrite(trig2, LOW);
   duration2 = pulseIn(echo2, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
   distance2= duration2*0.034/2;
   return  distance2;
  }
