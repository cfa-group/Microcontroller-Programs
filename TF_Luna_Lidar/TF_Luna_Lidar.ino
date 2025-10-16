#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial Serial1(2, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int dist; //actual distance measurements of LiDAR
int strength; //signal strength of LiDAR
float temprature;
int check; //save check value
int i;
int uart[9]; //save data measured by LiDAR
const int HEADER=0x59; //frame header of data package

byte pot = A0;
byte sel = 6;
int dpot = 0;
int outPot = 0;
int dsel = 0;
int r = 0;

void setup() {
  // put your setup code here, to run once:
  lcd.init(); 
  lcd.backlight();
  Serial.begin(9600);
  Serial1.begin(115200);

  
}

void loop(){
    if (outPot == 0 && dsel ==0){
        getDsel();
        if (outPot == 0 && dsel ==1){
           getDpot();
           if (outPot > 0 && dsel == 0){
              getDataSerial1();
              float phi = 3.14;
              float vol = phi*pow(outPot, 2)*dist;
              lcd.setCursor(3, 0);
              lcd.print(vol);
              lcd.setCursor(3, 1);
              lcd.print(strength);
              lcd.setCursor(12, 0);
              lcd.print(temprature);
              if (outPot > 0 && dsel == 1){
                getDsel();
                }
            }
        }
    }     
}


uint8_t getDsel(){
  lcd.setCursor(0, 0);
  lcd.print("Set1_Switch!");
  dsel = digitalRead(sel);
  if (dsel != 0){
    delay(1000);
    lcd.clear();
    }else{
      dsel = 0;
      }
  return dsel;  
  }

uint8_t getDpot(){
  dpot = digitalRead(pot);
  outPot = map(dpot, 0, 1023, 0, 255);
  lcd.setCursor(0, 0);
  lcd.print("Set r : ");
  lcd.print(outPot);
  if (outPot > 0 && dsel == 0){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("V: ");
      lcd.setCursor(0, 1);
      lcd.print("S: ");
      lcd.setCursor(9, 0);
      lcd.print("T: ");
  }
  return outPot;
  }  


uint8_t getDataSerial1(){
  if (Serial1.available()) { //check if serial port has data input
        if(Serial1.read() == HEADER) { //assess data package frame header 0x59
            uart[0]=HEADER;
            if (Serial1.read() == HEADER) { //assess data package frame header 0x59
                uart[1] = HEADER;
                for (i = 2; i < 9; i++) { //save data in array
                    uart[i] = Serial1.read();
                }
                check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
                if (uart[8] == (check & 0xff)){ //verify the received data as per protocol
                    dist = uart[2] + uart[3] * 256; //calculate distance value
                    strength = uart[4] + uart[5] * 256; //calculate signal strength value
                    temprature = uart[6] + uart[7] *256;//calculate chip temprature
                    temprature = temprature/8 - 256;
                    Serial.print("dist = ");
                    Serial.print(dist); //output measure distance value of LiDAR
                    Serial.print('\t');
                    Serial.print("strength = ");
                    Serial.print(strength); //output signal strength value
                    Serial.print("\t Chip Temprature = ");
                    Serial.print(temprature);
                    Serial.println(" celcius degree"); //output chip temperature of Lidar
                    return dist, strength, temprature;
                }
                
            }
            
        }
        
    }
  
 }






 
