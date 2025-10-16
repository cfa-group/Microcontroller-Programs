#include <LiquidCrystal_I2C.h>
#include <virtuabotixRTC.h> 
#include <EEPROM.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//                pin (CLK, DAT, RST)
virtuabotixRTC myRTC(6,7,8);

#define pot A0
#define relay 3
const int btn = 11;

int dataPot, dataBtn;


void setup(){
  Serial.begin(9600);
  lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("Bell Sekolah");
  pinMode(pot, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  digitalWrite(relay, LOW);
//  myRTC.setDS1302Time(00, 21, 14, 3, 07, 4, 2021);
  
  
  }

void loop(){
      btnRead();              
  }

void btnRead(){
  dataBtn = digitalRead(btn);
//  Serial.println(dataBtn);
  delay(100);
  if (dataBtn == HIGH){
    Serial.println("Low-Push_Btn...");
    delay(70);
    Serial.println("Head()...");
    Head();
    }else{
      delay(300);
      Serial.println("High-Push_Btn...");
      delay(100);
      Serial.println("call-Set_Function...");
      delay(100);
      Serial.println("Set()...");
      masuk();
      }
  }

//void Set(){
//  dataPot = analogRead(pot);
//  Jdat = map(dataPot, 0, 1021, 0, 24);
//  Serial.println(Jdat);
//  }


void Head(){
  myRTC.updateTime();                                                                                    //| 
  Serial.print("Current Date / Time: ");                                                                 //| 
  Serial.print(myRTC.dayofmonth);                                                                        //| 
  Serial.print("/");                                                                                     //| 
  Serial.print(myRTC.month);                                                                             //| 
  Serial.print("/");                                                                                     //| 
  Serial.print(myRTC.year);                                                                              //| 
  Serial.print("  ");                                                                                    //| 
  Serial.print(myRTC.hours);                                                                             //| 
  Serial.print(":");                                                                                     //| 
  Serial.print(myRTC.minutes);                                                                           //| 
  Serial.print(":");                                                                                     //| 
  Serial.println(myRTC.seconds);                                                                         //| 
  lcd.setCursor(0, 1);
  lcd.print(myRTC.dayofmonth);
  lcd.print('/');
  lcd.print(myRTC.month);
  lcd.print('/');
  lcd.print(myRTC.year);
  lcd.setCursor(0, 2);
  lcd.print(myRTC.hours);
  lcd.print(':');
  lcd.print(myRTC.minutes);
  lcd.print(':');
  lcd.print(myRTC.seconds);
  delay(1000);
  return;
  }

void masuk(){
  digitalWrite(relay, HIGH);
  delay(1000);
  digitalWrite(relay, LOW);
  delay(500);
  digitalWrite(relay, HIGH);
  delay(3000);
  digitalWrite(relay, LOW);
  }

void istrahat(){
  digitalWrite(relay, HIGH);
  delay(1000);
  digitalWrite(relay, LOW);
  delay(500);
  digitalWrite(relay, HIGH);
  delay(2000);
  digitalWrite(relay, LOW);
  }

void pulang(){
  digitalWrite(relay, HIGH);
  delay(2000);
  digitalWrite(relay, LOW);
  delay(500);
  digitalWrite(relay, HIGH);
  delay(3000);
  digitalWrite(relay, LOW);
  }  
