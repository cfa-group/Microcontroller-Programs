
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <SoftwareSerial.h>
#include <DS3231.h>
#include <Wire.h>
#include "TFMini.h"
TFMini tfmini;

DS3231 clock;
SoftwareSerial SerialTFMini(2, 3);          //The only value that matters here is the first one, 2, Rx
 
void getTFminiData(int* distance, int* strength)
{
  static char i = 0;
  char j = 0;
  int checksum = 0;
  static int rx[9];
  if (SerialTFMini.available())
  {
    rx[i] = SerialTFMini.read();
    if (rx[0] != 0x59)
    {
      i = 0;
    }
    else if (i == 1 && rx[1] != 0x59)
    {
      i = 0;
    }
    else if (i == 8)
    {
      for (j = 0; j < 8; j++)
      {
        checksum += rx[j];
      }
      if (rx[8] == (checksum % 256))
      {
        *distance = rx[2] + rx[3] * 256;
        *strength = rx[4] + rx[5] * 256;
      }
      i = 0;
    }
    else
    {
      i++;
    }
  }
}
 

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

//=====================================================================

void setup()
{
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);       //Initialize hardware serial port (serial debug port)
  while (!Serial);            // wait for serial port to connect. Needed for native USB port only
  Serial.println ("Initializing...");
  SerialTFMini.begin(TFMINI_BAUDRATE);    //Initialize the data rate for the SoftwareSerial port
  tfmini.begin(&SerialTFMini);            //Initialize the TF Mini sensor
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

//===================================================================
 
void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("V: ");
  lcd.setCursor(10, 1);
  lcd.print("t:");
  
  lcd.setCursor(0, 1);
  lcd.print("Cm Kubik");
  
  lcd.setCursor(10, 0);
  lcd.print("c:");
  tmp = clock.getTemperature();
  lcd.setCursor (12, 0);
  lcd.print(tmp);
  int distance = 0;
  int strength = 0;
  V = 0;
 
  getTFminiData(&distance, &strength);
  while (!distance)
  {
    getTFminiData(&distance, &strength);
    if (distance)
    {
      Serial.print(distance);
      Serial.print("cm\t");
      Serial.print("strength: ");
      Serial.println(strength);
      V = phi * r2 * distance;
      
      lcd.setCursor(3, 0);
      lcd.print(V);
      lcd.setCursor(12, 1);
      lcd.print(distance);
    }
  }
  delay(500);
  lcd.clear();
}
