
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <virtuabotixRTC.h>

//Inisialisasi pin (CLK, DAT, RST)
virtuabotixRTC myRTC(3,2,8);

LiquidCrystal_I2C lcd(0x27, 16, 2); 


bool stat = false;
int dataa, 
    data1, 
    data2, 
    data, 
    d3,
    curHour,
    curMin,
    curSec,
    dPot = 0, 
    dwtr1 = 0, 
    dwtr2 = 0; 
    

#define swtr1  A1
#define swtr2  A0
#define pot  A2

#define relayIn   5
#define relayOut  6

#define s3  9






uint8_t readPot1(){
  dPot = analogRead(pot);
  dataa = map(dPot, 0, 1023, 1, 24);
  return dataa;
  }

uint8_t readPot2(){
  dPot = analogRead(pot);
  data = map(dPot, 0, 1023, 1, 24);
  return data;
  }

uint8_t swPot(){
  d3 = digitalRead(s3);
  return d3;
  }

  



void setup() {
  // put your setup code here, to run once:

  

  
  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Pakan Air Ternak");
  lcd.setCursor(0, 1);
  lcd.print("Berbasis Arduino");
  delay(3000);
  lcd.clear();
  
  pinMode(relayOut, OUTPUT);
  pinMode(relayIn, OUTPUT);
  digitalWrite(relayIn, HIGH);
  digitalWrite(relayOut, HIGH);
  delay(1500);
  digitalWrite(relayIn, LOW);
  digitalWrite(relayOut, LOW);

  pinMode(s3, INPUT_PULLUP);
  
  Serial.println("Start in Setup(true) Func...");
  
//  myRTC.setDS1302Time(20, 20, 20, 6, 15, 2, 2022); //line code comment after set Time.
  lcd.setCursor(0, 0);
  lcd.print("Set_On_SwPot...");
  lcd.setCursor(0, 1);
  lcd.print("For_Setup-dTime");
  delay(2500);
  while(!stat){
    
    d3 = swPot();
    Serial.print("Swpot : ");
    Serial.println(d3);
    
    if (d3 == HIGH && data1 < 1 && data2 < 1){
      
      dataa = readPot1();
      Serial.print("Data 1 Pot :   ");
      Serial.println(dataa);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("dTime1 : ");
      lcd.setCursor(9, 0);
      lcd.print(dataa);
      
      d3 = swPot();
      Serial.print("Swpot : ");
      Serial.println(d3);
      
    }
    
    if (d3 == LOW && dataa > 1 && data2 < 1){
      
      data1 = dataa;
      data = readPot2();
      Serial.print("Data 2 Pot :   ");
      Serial.println(data);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("dTime1 : ");
      lcd.setCursor(9, 0);
      lcd.print(data1);
      lcd.setCursor(0, 1);
      lcd.print("dTime2 : ");
      lcd.setCursor(9, 1);
      lcd.print(data);
      
      d3 = swPot();
      Serial.print("Swpot : ");
      Serial.println(d3);
      
    }
    
    if (d3 == HIGH && data1 > 1 && data2 < 1){
      
      data2 = data;
      Serial.print("data_1 :  ");
      Serial.println(data1);
      Serial.print("data_2 :  ");
      Serial.println(data2);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Data:");
      lcd.setCursor(5, 0);
      lcd.print("Time1 : ");
      lcd.setCursor(13, 0);
      lcd.print(data1);
      lcd.setCursor(5, 1);
      lcd.print("Time2 : ");
      lcd.setCursor(13, 1);
      lcd.print(data2);
      
      Serial.println();
      Serial.println("Start in Loop(true) Func...");  
      delay(2000);
      lcd.clear();
      stat = true;
      
    }
      
    delay(500);
    
    }

    
}

void loop() {
  // put your main code here, to run repeatedly:
  myRTC.updateTime();  
  getTime();
  getSensor();
  
  
  Serial.println("\t------------------------GetStatTime------------------------");
  Serial.print("\tGet-Data_Stat =   ");
  Serial.println(stat);
  
  Serial.print("\tdataTime_1 :  ");
  Serial.println(data1);
  Serial.print("\tdataTime_2 :  ");
  Serial.println(data2);

  Serial.println();
  Serial.println();
  Serial.println();
  
  Serial.println("\t--------------------------GetData--------------------------");
  Serial.println("\tdata_Swtr1\tdata_Swtr2\t\tTime");
  Serial.print("\t"); 
  Serial.print(dwtr1);  
  Serial.print("\t"); 
  Serial.print("\t"); 
  Serial.print(dwtr2); 
  Serial.print("\t");
  Serial.print("\t"); 
  Serial.print("\t"); 
  Serial.print(curHour);
  Serial.print(" : ");
  Serial.print(curMin);
  Serial.print(" : ");
  Serial.println(curSec);
    
    if (dwtr1 < 100 && dwtr2 < 100){
        digitalWrite(relayIn, HIGH);
        }else{
          digitalWrite(relayIn, LOW);
          }

    
      
    if (curHour == data1 && curMin < 1 && curSec < 5){
        exe();
        }else{
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Wtr1:");
            lcd.setCursor(5, 0);
            lcd.print(dwtr1);
            lcd.setCursor(0, 1);
            lcd.print("Wtr2:");
            lcd.setCursor(5, 1);
            lcd.print(dwtr2);
  
            lcd.setCursor(11, 0);
            lcd.print(curHour);
            lcd.setCursor(13, 0);
            lcd.print(":");
            lcd.setCursor(14, 0);
            lcd.print(curMin);
            lcd.setCursor(14, 1);
            lcd.print(curSec);
            
            }

    if (curHour == data2 && curMin < 1 && curSec < 5){
          exe();
          }else{
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Wtr1:");
            lcd.setCursor(5, 0);
            lcd.print(dwtr1);
            lcd.setCursor(0, 1);
            lcd.print("Wtr2:");
            lcd.setCursor(5, 1);
            lcd.print(dwtr2);
  
            lcd.setCursor(11, 0);
            lcd.print(curHour);
            lcd.setCursor(13, 0);
            lcd.print(":");
            lcd.setCursor(14, 0);
            lcd.print(curMin);
            lcd.setCursor(14, 1);
            lcd.print(curSec);
            
            }
  
  Serial.println();
  Serial.println();
  Serial.println();
  
  delay(1000);
  
}




uint8_t getSensor(){
    dwtr1 = analogRead(swtr1);
    dwtr2 = analogRead(swtr2);
    
    return dwtr1, dwtr2;
      
  }

uint8_t getTime(){  
  curHour = myRTC.hours;
  curMin  = myRTC.minutes;
  curSec  = myRTC.seconds;
  
  return curHour, curMin, curSec;
  
}

uint8_t exe(){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Out True...");
        digitalWrite(relayOut, HIGH);
        delay(10000);
        digitalWrite(relayOut, LOW);
        delay(1500);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Out True...");
        digitalWrite(relayIn, HIGH);
        delay(10000);
        digitalWrite(relayIn, LOW);
//        lcd.clear();
  }
