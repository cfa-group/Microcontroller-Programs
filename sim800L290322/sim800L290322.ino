#include <Wire.h>
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include <RTClib.h>
#include <SoftwareSerial.h>
#include "ACS712.h"

RTC_DS3231 RTC;
//sender phone number with country code
const String PHONE = "+6281234540680";

//GSM Module RX pin to Arduino 3
//GSM Module TX pin to Arduino 2
#define rxPin 2
#define txPin 3
SoftwareSerial sim800(rxPin,txPin);
String smsStatus,senderNumber,receivedDate,msg;
boolean isReply = false;

#define ch1  7
#define ch2  6
#define ch3  5
#define ch4  4

ACS712 sensor(ACS712_30A, A1);
float U,
      I,
      P;

#define sldr A1
int dataLDR;
int Set = 0;
int p ;

void setup() {
  
//  sensor.calibrate();
//
//  pinMode(ch1, OUTPUT); //Relay 1
//  pinMode(ch2, OUTPUT); //Relay 2
//  pinMode(ch3, OUTPUT); //Relay 1
//  pinMode(ch4, OUTPUT); //Relay 2
//  
//  digitalWrite(ch1, HIGH);
//  digitalWrite(ch2, HIGH);
//  digitalWrite(ch3, HIGH);
//  digitalWrite(ch4, HIGH);
  
  delay(7000);
  
  Serial.begin(57600);
//  Serial.println("Arduino serial initialize");
  
  sim800.begin(9600);
//  Serial.println("SIM800L software serial initialize");
//  Wire.begin();
//  RTC.begin();

//  RTC.adjust(DateTime(__DATE__, __TIME__));
//  if (! RTC.isrunning()) {
//    Serial.println("RTC is NOT running!");
//    // following line sets the RTC to the date & time this sketch was compiled
//    RTC.adjust(DateTime(__DATE__, __TIME__));
//  }

  smsStatus = "";
  senderNumber="";
  receivedDate="";
  msg="";

  sim800.println("AT");
  ShowSerialData();
  sim800.println("ATI");
  ShowSerialData();
  sim800.println("AT+COPS?");
  ShowSerialData();
  sim800.println("AT+CBC");
  ShowSerialData();
  sim800.println("AT+CSQ");
  ShowSerialData();
  sim800.println("AT+CCID");
  ShowSerialData();

  // Check whether it has registered on the network
  sim800.println("AT+CREG?");
  ShowSerialData();
  sim800.println("AT+CMGF=1");
  ShowSerialData();
  sim800.println("AT+CNMI=1,2,0,0,0");
  ShowSerialData();
  sim800.println("AT+CMGD=?");
  ShowSerialData();
  sim800.println("AT+CMGD=21,4");
  ShowSerialData();

//  ShowSerialData();
  

}

uint8_t ShowSerialData()
{
  while(sim800.available()!=0)
  Serial.write(sim800.read());
  delay(1000); 
  
}

void loop() {
    //////////////////////////////////////////////////
    while(sim800.available()){
      parseData(sim800.readString());
    }
    //////////////////////////////////////////////////
    while(Serial.available())  {
      sim800.println(Serial.readString());
    }
    //////////////////////////////////////////////////
//    delay(1000);
//    getTime();
//    GetLDR();
//    cur();
    


} //main loop ends




//***************************************************
void parseData(String buff){
  Serial.println(buff);

  unsigned int len, index;
  //////////////////////////////////////////////////
  //Remove sent "AT Command" from the response string.
  index = buff.indexOf("\r");
  buff.remove(0, index+2);
  buff.trim();
  //////////////////////////////////////////////////
  
  //////////////////////////////////////////////////
  if(buff != "OK"){
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();
    
    buff.remove(0, index+2);
    
    if(cmd == "+CMT"){
      //get newly arrived memory location and store it in temp
      index = buff.indexOf(",");
      String temp = buff.substring(index+1, buff.length()); 
      temp = "AT+CMGR=" + temp + "\r"; 
      //get the message stored at memory location "temp"
      sim800.println(temp); 
    }
    else if(cmd == "+CMGR"){
      extractSms(buff);
      
      if(senderNumber == PHONE){
        
        doAction();
      }
    }
  //////////////////////////////////////////////////
  }
  else{
  //The result of AT Command is "OK"
  }
}




//************************************************************
void extractSms(String buff){
   unsigned int index;
   
    index = buff.indexOf(",");
    smsStatus = buff.substring(1, index-1); 
    buff.remove(0, index+2);
    
    senderNumber = buff.substring(0, 13);
    buff.remove(0,19);
   
    receivedDate = buff.substring(0, 20);
    buff.remove(0,buff.indexOf("\r"));
    buff.trim();
    
    index =buff.indexOf("\n\r");
    buff = buff.substring(0, index);
    buff.trim();
    msg = buff;
    buff = "";
    msg.toLowerCase();
}



void doAction(){
  Serial.println("do_Action_parameter:");
  Serial.print("Req_Msg: ");
  Serial.println(msg);
  
  if(msg == "info"){  
    Reply("info");
  }
  else if(msg == "Safe_Mode"){
    digitalWrite(ch1, LOW);
    digitalWrite(ch2, HIGH);
    digitalWrite(ch3, HIGH);
    digitalWrite(ch4, LOW);
    Reply("Safe_Mode");
  }
  else if(msg == "Night_Mode"){
    digitalWrite(ch1, HIGH);
    digitalWrite(ch2, HIGH);
    digitalWrite(ch3, LOW);
    digitalWrite(ch4, LOW);
    Reply("Night_Mode");
  }
  else if(msg == "Morning_Mode"){
    digitalWrite(ch1, LOW);
    digitalWrite(ch2, LOW);
    digitalWrite(ch3, HIGH);
    digitalWrite(ch4, HIGH);
    Reply("Morning_Mode");
  }
  else if(msg == "Def_Mode"){
    digitalWrite(ch1, LOW);
    digitalWrite(ch2, LOW);
    digitalWrite(ch3, LOW);
    digitalWrite(ch4, LOW);
    Reply("Def_Mode");
  }

  
  smsStatus = "";
  senderNumber="";
  receivedDate="";
  msg="";  
}



void Reply(String text)
{
    sim800.print("AT+CMGF=1\r");
    delay(1000);
    sim800.print("AT+CMGS=\""+PHONE+"\"\r");
    delay(1000);
    sim800.print(text);
    delay(100);
    sim800.write(0x1A); //ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
    delay(1000);
    Serial.println("SMS Sent Successfully.");
}



void GetLDR(){
  dataLDR = analogRead(sldr);
  Serial.print("Nilai LDR: ");
  Serial.println(dataLDR);
  Serial.println();
  delay(1000);
  }



void cur(){
  U = 230;

  // To measure current we need to know the frequency of current
  // By default 50Hz is used, but you can specify own, if necessary
  I = sensor.getCurrentAC();

  // To calculate the power we need voltage multiplied by current
  P = U * I;

  Serial.println(String("P = ") + P + " Watts");
  Serial.println();
  Serial.println();
  delay(1000);
  }



void getTime(){
    DateTime now = RTC.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.println();
    delay(1000);
  }

  
