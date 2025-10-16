/* 
  Pir 1 =====> D2
  Pir 2 =====> D3

              Trig   Echo
  HcSr04 ===> A0      A1
              A2      A3
              A4      A5

  LM Driver Motor DC In1      D5
                     In2      D6
                     In3      D7
                     In4      D8

*/

const byte trig[] = {A0, A2, A4};
const byte echo[] = {A1, A3, A5};
const byte In[]   = {5, 6, 7, 8};
const byte pir[]  = {2, 3}; // [0] detect montion front


int pirVallFront, pirVallBehind;
long duration0, dist0, duration1, dist1, duration2, dist2;

unsigned long interval = 500; // the time we need to wait
long previousMillis = 0; // millis() returns an unsigned long.


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 
    pinMode(pir[0], INPUT);
    pinMode(pir[1], INPUT);

    pinMode(trig[0], OUTPUT);
    pinMode(echo[0], INPUT);
    pinMode(trig[1], OUTPUT);
    pinMode(echo[1], INPUT);
    pinMode(trig[2], OUTPUT);
    pinMode(echo[2], INPUT);

    pinMode(In[0], OUTPUT);
    pinMode(In[1], OUTPUT);
    pinMode(In[2], OUTPUT);
    pinMode(In[3], OUTPUT);
    

  Serial.println("Load Setup Success...");
  Serial.println("Start...");

}

void loop() {
  // put your main code here, to run repeatedly:

unsigned long currentMillis = millis(); 
if ((unsigned long)(currentMillis - previousMillis) >= interval) {

    Serial.println("|++++++DETECT_ITEM++++++|+++++++++++++++> VALLUE <+++++++++++++++|");
    GetPirFront();
    Serial.print("| PirFront[0]-ReadVall  |========>    ");
    Serial.println(pirVallFront);

    // GetPirBehind();
    Serial.print("| PirBehind[1]-ReadVall |========>    ");
    Serial.println(pirVallBehind);
   
    //----------------------------------HCSR04------------------------------------//

    Getdist0();
    Serial.print("| HC-SR04[0]-ReadVall   |========>    ");
    Serial.print(dist0);
    Serial.println(" Cm");

    Getdist1();
    Serial.print("| HC-SR04[1]-ReadVall   |========>    ");
    Serial.print(dist1);
    Serial.println(" Cm");

    Getdist2();
    Serial.print("| HC-SR04[2]-ReadVall   |========>    ");
    Serial.print(dist2);
    Serial.println(" Cm");
    Serial.println("|__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__-__|");

    previousMillis = millis();
  }
  
} 


uint8_t GetPirFront(){
  pirVallFront = digitalRead(pir[0]);
  if(pirVallFront == 1){
    stp();
    delay(5000);
    pirVallBehind = digitalRead(pir[1]);
    if (pirVallBehind == 1){
        stp();
        delay(3000);
    }
    
  }
  return pirVallFront, pirVallBehind;
}

uint8_t Getdist0(){
  digitalWrite(trig[0], LOW);delayMicroseconds(2);
  digitalWrite(trig[0], HIGH);delayMicroseconds(10);
  digitalWrite(trig[0], LOW);
  duration0 = pulseIn(echo[0], HIGH);
  dist0 = duration0/58.2;
  if(dist0 < 20){
    left();
  }else{
    forward();
  }
  return dist0;
}

uint8_t Getdist1(){
  digitalWrite(trig[1], LOW);delayMicroseconds(2);
  digitalWrite(trig[1], HIGH);delayMicroseconds(10);
  digitalWrite(trig[1], LOW);
  duration1 = pulseIn(echo[1], HIGH);
  dist1 = duration1/58.2;
  if(dist1 < 20){
    right();
  }else{
    forward();
  }
  return dist1;
}

uint8_t Getdist2(){
  digitalWrite(trig[2], LOW);delayMicroseconds(2);
  digitalWrite(trig[2], HIGH);delayMicroseconds(10);
  digitalWrite(trig[2], LOW);
  duration2 = pulseIn(echo[2], HIGH);
  dist2 = duration2/58.2;
  if(dist2 < 20){
    right();
  }else{
    forward();
  }
  return dist2;
}


void forward(){
  digitalWrite(In[0], LOW);
  digitalWrite(In[1], HIGH);
  digitalWrite(In[2], LOW);
  digitalWrite(In[3], HIGH);
  

  }

void backof(){
  digitalWrite(In[0], HIGH);
  digitalWrite(In[1], LOW);
  digitalWrite(In[2], HIGH);     //Mundur func.
  digitalWrite(In[3], LOW);
  

  }
  
void left(){
  digitalWrite(In[0], HIGH);
  digitalWrite(In[1], LOW);
  digitalWrite(In[2], LOW);
  digitalWrite(In[3], HIGH);
  
  }


//-- balik kanan
void right(){
  digitalWrite(In[0], LOW);
  digitalWrite(In[1], HIGH);
  digitalWrite(In[2], HIGH);
  digitalWrite(In[3], LOW);
  
  }  

void stp(){
  digitalWrite(In[0], LOW);
  digitalWrite(In[1], LOW);
  digitalWrite(In[2], LOW);
  digitalWrite(In[3], LOW);
  
  }
