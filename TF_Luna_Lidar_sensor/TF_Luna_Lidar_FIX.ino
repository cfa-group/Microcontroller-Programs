

#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>   //header file of software serial port
#include <DS3231.h>
#include <Wire.h>

DS3231 clock;
SoftwareSerial Serial1(2, 3); //define software serial port name as Serial1 and define pin2 as RX & pin3 as TX
 
int dist;                     //actual distance measurements of LiDAR
int strength;                 //signal strength of LiDAR
int check;                    //save check value
int I;
int uart[9];                   //save data measured by LiDAR
const int HEADER = 0x59;      //frame header of data package
 
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define pot  A2
#define sw   9
bool stat = false;
int dPot = 0,
    t = 0, 
    d = 0,
    dataP = 0,
    r = 0,
    r2 = 0,
    V = 0,
    s, m, h, tmp,
    dsw = 0;

float phi = 3.14 ;    

uint8_t readPot(){
  dPot = analogRead(pot);
  d = map(dPot, 0, 1023, 1, 800);
  return d, dPot;
  }

uint8_t swPot(){
  dsw = digitalRead(sw);
  return dsw;
  }
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();      
  Serial1.begin(115200);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("=====TFLuna=====");
  lcd.setCursor(0, 1);
  lcd.print("=LIDAR--ARDUINO=");
  delay(3000);
 


  
  lcd.clear();
  pinMode(sw, INPUT_PULLUP);
  lcd.setCursor(0, 0);
  lcd.print("Set_On_switch...");
  lcd.setCursor(0, 1);
  lcd.print("For_Setup(d & r)");
  delay(2500);

  while(!stat){
    
    dsw = swPot();
    Serial.print("Sw : ");
    Serial.println(dsw);
    
    if (dsw == LOW && dataP < 1){
      
      d = readPot();
      Serial.print("pot : ");
      Serial.println(d);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Diameter: ");
      lcd.setCursor(10, 0);
      lcd.print(d);
//      lcd.setCursor(10, 1);
//      lcd.print(dPot);
      dsw = swPot();
      Serial.print("Sw : ");
      Serial.println(dsw);
      
      }
      
    if (dsw == HIGH && d > 0){
      dataP = d;
      r = dataP/2;
      r2 = pow(r, 2);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Processing...");
      lcd.setCursor(0, 1);
      lcd.print("d/2: ");
      lcd.setCursor(4, 1);
      lcd.print(r);
      
      lcd.setCursor(8, 1);
      lcd.print("r:");
      lcd.setCursor(10, 1);
      lcd.print(r2);
      delay(3000);
      lcd.clear();
      stat = true;
      }
      delay(500);
    }



  delay(2000);
  lcd.clear();

  
}

void loop() {
  // put your main code here, to run repeatedly:

  lcd.setCursor(0, 0);
  lcd.print("V: ");
  lcd.setCursor(10, 1);
  lcd.print("t:");
  
  lcd.setCursor(0, 1);
  lcd.print("Cm Kubik");
  
  lcd.setCursor(10, 0);
//  lcd.print("c:");
//  tmp = clock.getTemperature();
  lcd.setCursor (12, 0);
//  lcd.print(tmp);
  
  if (Serial1.available())                //check if serial port has data input
    {
      if (Serial1.read() == HEADER)        //assess data package frame header 0x59
      {
        uart[0] = HEADER;
        if (Serial1.read() == HEADER)      //assess data package frame header 0x59
        {
          uart[1] = HEADER;
          for (I = 2; I < 9; I++)         //save data in array
          {
            uart[I] = Serial1.read();
          }
          check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
          if (uart[8] == (check & 0xff))        //verify the received data as per protocol
          {
            dist = uart[2] + uart[3] * 256;     //calculate distance value
            strength = uart[4] + uart[5] * 256; //calculate signal strength value
            Serial.print("dist = ");
            Serial.println(dist);                
            
            
//            V = phi * r2 * dist;
            
            
//            lcd.setCursor(3, 0);
//            lcd.print(V);
            lcd.setCursor(12, 1);
            lcd.print(dist);
            
            
          }
        }
      }
    }


//delay(500);
//lcd.clear();



  
}
