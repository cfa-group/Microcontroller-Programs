// Date, Time and Alarm functions using a DS3231 RTC connected via I2C and Wire lib

#include <Wire.h>
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include <RTClib.h>
#include <SoftwareSerial.h>

RTC_DS3231 RTC;
const String PHONE = "+6281234540680";

#define PIN_TX  10
#define PIN_RX  11

SoftwareSerial serial(PIN_TX, PIN_RX);

void setup () {
    Serial.begin(57600);
    serial.begin(9600);
//    Wire.begin();
//    RTC.begin();
//    
//  RTC.adjust(DateTime(__DATE__, __TIME__));
//  if (! RTC.isrunning()) {
//    Serial.println("RTC is NOT running!");
//    // following line sets the RTC to the date & time this sketch was compiled
//    RTC.adjust(DateTime(__DATE__, __TIME__));
//  }

  
  
//  sensor.calibrate();
  serial.println("AT");
  ShowSerialData();
  serial.println("ATI");
  ShowSerialData();
  serial.println("AT+COPS?");
  ShowSerialData();
  serial.println("AT+CBC");
  ShowSerialData();
  serial.println("AT+CSQ");
  ShowSerialData();
  serial.println("AT+CCID");
  ShowSerialData();

  // Check whether it has registered on the network
  serial.println("AT+CREG?");
  ShowSerialData();
  serial.println("AT+CMGF=1");
  ShowSerialData();
  serial.println("AT+CNMI=1,2,0,0,0");
  ShowSerialData();
  serial.println("AT+CMGD=?");
  ShowSerialData();
  serial.println("AT+CMGD=21,4");
  ShowSerialData();
//  pinMode(ch1, OUTPUT);
//  pinMode(ch2, OUTPUT);
//  pinMode(ch3, OUTPUT);
//  pinMode(ch4, OUTPUT);
//  digitalWrite(ch1, HIGH);
//  digitalWrite(ch2, HIGH);
//  digitalWrite(ch3, HIGH);
//  digitalWrite(ch4, HIGH);
//  dataLDR = 0;
//
//  p=0;
//  smsStatus = "";
//  senderNumber="";
//  receivedDate="";
//  msg="";
 
}

void loop () {
//    DateTime now = RTC.now();
//    
//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(' ');
//    Serial.print(now.hour(), DEC);
//    Serial.print(':');
//    Serial.print(now.minute(), DEC);
//    Serial.print(':');
//    Serial.print(now.second(), DEC);
//    Serial.println();
//
//    if (RTC.checkIfAlarm(1)) {
//      Serial.println("Alarm Triggered");
//    }
//    Serial.println();
//    delay(3000);

ShowSerialData();
}

uint8_t ShowSerialData()
{
  while(serial.available()!=0)
  Serial.write(serial.read());
  delay(1000); 
  
}
