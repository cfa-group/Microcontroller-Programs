
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <virtuabotixRTC.h>

//Inisialisasi pin (CLK, DAT, RST)
virtuabotixRTC myRTC(3,2,8);

LiquidCrystal_I2C lcd(0x27, 16, 2); 


#define relayIn   4
#define relayOut  5

#define wtr1 A0
#define wtr2 A1
int dwtr1;
int dwtr2;
#define pot  A2
#define potpwr  9
#define Swin   6  //read
#define Swout  7  //read
#define Swpot  8  //read

int dPot;
int dSwPot;
int dSwin;
int dSwout;
int mapPot;

int dataTime;
int CdataTime = 0;
int dt1 = 0;
int dt2 = 0;
int curHour;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 

  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Pakan Air Ternak");
  lcd.setCursor(0, 1);
  lcd.print("Berbasis Arduino");
  delay(3500);
  lcd.clear();
  pinMode(potpwr, OUTPUT);
  pinMode(Swin, INPUT);
  pinMode(Swout, INPUT);
  pinMode(Swpot, INPUT);
  pinMode(relayOut, OUTPUT);
  pinMode(relayIn, OUTPUT);
  digitalWrite(potpwr, HIGH);
//  myRTC.setDS1302Time(00,59,23,6,20,10,2017);
}

void loop() {
  // put your main code here, to run repeatedly:

  dSwin  = digitalRead(Swin);
  dSwout = digitalRead(Swout);
  dSwPot = digitalRead(Swpot);
  Serial.print("In  : ");
  Serial.println(dSwin);
  Serial.print("Out : ");
  Serial.println(dSwout);
  Serial.print("Swpot : ");
  Serial.println(dSwPot);
  
//---------------------------------------------------------------------------------------------

  if (dSwin == HIGH && dSwout == LOW && dSwPot == LOW){
    digitalWrite(relayIn, HIGH);
    digitalWrite(relayOut, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Water_In...");
    }
      
//---------------------------------------------------------------------------------------------

  if (dSwout == HIGH && dSwin == LOW && dSwPot == LOW){
    digitalWrite(relayIn, LOW);
    digitalWrite(relayOut, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Water_Out...");
    }

//---------------------------------------------------------------------------------------------

  if (dSwout == HIGH && dSwin == HIGH && dSwPot == LOW){
    digitalWrite(relayIn, HIGH);
    digitalWrite(relayOut, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Water_In...");
    lcd.setCursor(0, 1);
    lcd.print("Water_Out...");
    }

//---------------------------------------------------------------------------------------------

  if (dSwout == LOW && dSwin == LOW && dSwPot == LOW){
    digitalWrite(relayIn, LOW);
    digitalWrite(relayOut, LOW);

//    delay(1000);
    }
    
//---------------------------------------------------------------------------------------------

  if (dSwPot == HIGH && dSwout == LOW && dSwin == LOW){
    dataTime = readPot();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("dataTime : ");
    lcd.setCursor(0, 1);
    lcd.print(dataTime);
    dSwPot = digitalRead(Swpot);
    Serial.println(dataTime);
    
  }else if(dSwPot == HIGH && dSwout == HIGH && dSwin == LOW){
    dataTime = readPot();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("dataTime : ");
    lcd.setCursor(0, 1);
    lcd.print(dataTime);
    dSwPot = digitalRead(Swpot);
    Serial.println(dataTime);
    
    }else if(dSwPot == HIGH && dSwout == LOW && dSwin == HIGH){
      dataTime = readPot();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("dataTime : ");
      lcd.setCursor(0, 1);
      lcd.print(dataTime);
      dSwPot = digitalRead(Swpot);
      Serial.println(dataTime);
      
      }else if(dSwPot == HIGH && dSwout == HIGH && dSwin == HIGH){
        dataTime = readPot();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("dataTime : ");
        lcd.setCursor(11, 0);
        lcd.print(dataTime);
        dSwPot = digitalRead(Swpot);
        Serial.println(dataTime);
        
        }

//---------------------------------------------------------------------------------------------
  
  if (dSwPot == LOW && CdataTime != dataTime){
    
    Serial.println("stat : SwPot = Low && cdataTime != dataTime");

    
    CdataTime = dataTime;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DSetUp: ");
    lcd.setCursor(8, 0);
    lcd.print(CdataTime);
//    delay(2500);
    
//    ModeAuto();
//    lcd.clear();
//    lcd.setCursor(0, 0);
//    lcd.print("Wtr1: ");
//    lcd.setCursor(6, 0);
//    lcd.print(dwtr1);
//    lcd.setCursor(0, 1);
//    lcd.print("Wtr2: ");
//    lcd.setCursor(6, 1);
//    lcd.print(dwtr2);
//    lcd.setCursor(11, 1);
//    lcd.print("cT:");
//    lcd.setCursor(14, 0);
//    lcd.print(CdataTime);

    }

//---------------------------------------------------------------------------------------------
      
  if (dSwPot == 0 && CdataTime == dataTime){
    Serial.println("stat : SwPot = Low && cdataTime = dataTime");

    
    CdataTime = dataTime;
    ModeAuto();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wtr1: ");
    lcd.setCursor(6, 0);
    lcd.print(dwtr1);
    lcd.setCursor(0, 1);
    lcd.print("Wtr2: ");
    lcd.setCursor(6, 1);
    lcd.print(dwtr2);
    lcd.setCursor(11, 1);
    lcd.print("cT:");
    lcd.setCursor(14, 0);
    lcd.print(CdataTime);

   }
    
//---------------------------------------------------------------------------------------------
  
  
  delay(500);


//---------------------------------------------------------------------------------------------


}















uint8_t getWtr(){
  dwtr1 = analogRead(wtr1);
  dwtr2 = analogRead(wtr2);
  return dwtr1, dwtr2;
  }

uint8_t readPot(){
  dPot = analogRead(pot);
  dataTime = map(dPot, 0, 1023, 1, 6);
  return dataTime;
  }

uint8_t ModeAuto(){
  dwtr1 = analogRead(wtr1);
  dwtr2 = analogRead(wtr2);
  return dwtr1, dwtr2;
  }
