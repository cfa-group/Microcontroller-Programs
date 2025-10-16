#define pirDepan  7
int statPD = 0;
#define pirTengah  8
int statPT = 0;

#define trig1  A0
#define echo1  A1
#define trig2  A2
#define echo2  A3
#define trig3  A4
#define echo3  A5
float duration1, dist1, duration2, dist2, duration3, dist3;
float maxdist = 20;

boolean Run = false;

#define in1  9 //+knn
#define in2  10 //-knn

#define in3  11 //-kiri
#define in4  12 //+ kiri

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trig1, OUTPUT);   //Depan
  pinMode(echo1, INPUT);
  
  pinMode(trig2, OUTPUT);   //Kanan
  pinMode(echo2, INPUT);
  
  pinMode(trig3, OUTPUT);   //Kiri
  pinMode(echo3, INPUT);
  pinMode(pirDepan, INPUT_PULLUP);
  pinMode(pirTengah, INPUT_PULLUP);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  

  delay(300);


  
  statPD = pirdpn();
  statPT = pirtngh();
  
  Serial.print("Stat Pir Depan : ");
  Serial.println(statPD);
  Serial.print("Stat Pir Tengah : ");
  Serial.println(statPT);

  if (statPD != 0 || statPT != 0){
    stp();
    delay(6500);
    runn();

  }else{
    runn();
    }



  
//  if (statPD == 1 || statPT == 1){
//    stp();
//    delay(3500);
//    }else{
//      hcdepan();
//      if (dist1 <= maxdist){
//        Serial.print("Dist Depan : ");
//        Serial.println(dist1);
//        hckanan();
//        if (dist2 <= maxdist){
//          Serial.print("Dist kanan : ");
//          Serial.println(dist2);
//          hckiri();
//          if (dist3 <= maxdist){
//            Serial.print("Dist kiri : ");
//            Serial.println(dist3);
//              
//            }else{
//              left();
//          
//              }
//            
//          }else{
//            right();
//      
//            }
//        
//        }else{
//          forward();
//       
//          }
//
//      }

    
}

uint8_t runn(){
  
  dist1 = hcdepan();
  dist2 = hckanan();
  dist3 = hckiri();
  Serial.print("Dist Depan : ");
  Serial.println(dist1);
  Serial.print("Dist kanan : ");
  Serial.println(dist2);
  Serial.print("Dist kiri : ");
  Serial.println(dist3);
  if (dist1 > maxdist && dist2 > maxdist && dist3 > maxdist){
    forward();
    }
    
    if (dist1 > maxdist && dist2 < maxdist && dist3 < maxdist){
      
        stp();
        delay(1000);
        back();
        delay(1200);
        right();
        }
      
      if (dist1 > maxdist && dist2 > maxdist && dist3 < maxdist){
        
      right();
      }

      if (dist1 < maxdist && dist2 > maxdist && dist3 < maxdist){
        
      right();
      }

      if (dist1 < maxdist && dist2 < maxdist && dist3 > maxdist){
        
      left();
      }

      if (dist1 > maxdist && dist2 < maxdist && dist3 > maxdist){
        
      left();
      }

      if (dist1 < maxdist && dist2 > maxdist && dist3 > maxdist){
        
        stp();
        delay(1000);
        back();
        delay(1500);
        right();
        
      }
        
      if (dist1 < maxdist && dist2 < maxdist && dist3 < maxdist){
        stp();
        delay(1000);
        back();
        delay(1500);
        right();
        }
   

  }

uint8_t hcdepan(){
  digitalWrite(trig1, LOW);delayMicroseconds(2);
  digitalWrite(trig1, HIGH);delayMicroseconds(10);
  digitalWrite(trig1, LOW);
  duration1 = pulseIn(echo1, HIGH);
  dist1 = duration1/58.2;
  return dist1;
  }

uint8_t hckanan(){
  digitalWrite(trig2, LOW);delayMicroseconds(2);
  digitalWrite(trig2, HIGH);delayMicroseconds(10);
  digitalWrite(trig2, LOW);
  duration2 = pulseIn(echo2, HIGH);
  dist2 = duration2/58.2;
  return dist2;
  }

uint8_t hckiri(){
  digitalWrite(trig3, LOW);delayMicroseconds(2);
  digitalWrite(trig3, HIGH);delayMicroseconds(10);
  digitalWrite(trig3, LOW);
  duration3 = pulseIn(echo3, HIGH);
  dist3 = duration3/58.2;
  return dist3;
  }      

uint8_t pirdpn(){
  statPD = digitalRead(pirDepan);
  return statPD;
  }

uint8_t pirtngh(){
  statPT = digitalRead(pirTengah);
  return statPT;
  }  




uint8_t forward(){
  Serial.println("Maju");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  
  }

uint8_t left(){
  Serial.println("Kiri");
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  
  }

uint8_t right(){
  Serial.println("Kanan");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);

  
  }

uint8_t stp(){
  Serial.println("Stop");
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  
  }


uint8_t back(){
  Serial.println("Back");
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  
  }
