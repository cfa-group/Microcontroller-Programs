#include <Wire.h> 
#include <SoftwareSerial.h>
//#include <OneWire.h>
#include <virtuabotixRTC.h>
#include "ACS712.h"

//Inisialisasi pin (CLK, DAT, RST)
virtuabotixRTC myRTC(6,7,8);
const String PHONE = "081234540680";

#define PIN_TX  10
#define PIN_RX  11

SoftwareSerial serial(PIN_TX, PIN_RX);

String smsStatus,senderNumber,receivedDate,msg;
boolean isReply = false;

#define ch1  12
#define ch2  5
#define ch3  4
#define ch4  3

ACS712 sensor(ACS712_30A, A1);
float I;

#define sldr A1
int dataLDR;
int Set = 0;
int p ;


void setup() {
  // put your setup code here, to run once:
  serial.begin(9600);
  Serial.begin(57600);
  sensor.calibrate();
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
  pinMode(ch1, OUTPUT);
  pinMode(ch2, OUTPUT);
  pinMode(ch3, OUTPUT);
  pinMode(ch4, OUTPUT);
  digitalWrite(ch1, HIGH);
  digitalWrite(ch2, HIGH);
  digitalWrite(ch3, HIGH);
  digitalWrite(ch4, HIGH);
  dataLDR = 0;

  p=0;
  smsStatus = "";
  senderNumber="";
  receivedDate="";
  msg="";
  //setup RTC
//  myRTC.setDS1302Time(00,14,15,1,20,03,2022);

}

void loop() {
 

  if(Serial.available() > 0){
    serial.write(Serial.read());
    }
  if(serial.available() > 0){
    msg = serial.readStringUntil('\n');
    Serial.print("Msg  : ");
    Serial.println(msg);
    if(msg == "Safe_Mode"){  
    digitalWrite(ch1, LOW);
    digitalWrite(ch2, HIGH);
    digitalWrite(ch3, HIGH);
    digitalWrite(ch4, LOW);
    p=("Safe_Mode Activated");
    
  }
  else if(msg == "Night_Mode"){
    digitalWrite(ch1, HIGH);
    digitalWrite(ch2, HIGH);
    digitalWrite(ch3, LOW);
    digitalWrite(ch4, LOW);
    Reply("Night_Mode Activated");
    p=2;
  }
  else if(msg == "Morning_Mode"){
    digitalWrite(ch1, LOW);
    digitalWrite(ch2, LOW);
    digitalWrite(ch3, HIGH);
    digitalWrite(ch4, HIGH);
    Reply("Morning_Mode Activated");
    p=3;
  }
  else if(msg == "Def_Mode"){
    digitalWrite(ch1, LOW);
    digitalWrite(ch2, LOW);
    digitalWrite(ch3, LOW);
    digitalWrite(ch4, LOW);
    Reply("Def_Mode Activated");
    p=4;
  }

  
  smsStatus = "";
  senderNumber="";
  receivedDate="";
  msg="";  
}

    }else{
      getTime();
      GetLDR();
      I = getAmp();
      Serial.println(String("I = ") + I + " Amp");
      }








//getTime();
//GetLDR();
//  I = getAmp();
//  Serial.println(String("I = ") + I + " Amp");

}

void SendData(String p){
  serial.print("AT+CMGF=1\r\n");
  delay(500);
  serial.print("AT+CMGS=\"");
  serial.print(PHONE);
  serial.print("\"\r\n");
  delay(2000);
  serial.print(p);
  serial.write(0x1A);
  delay(1000);
  serial.println("AT+CMGD=3");
  }



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
    
    if(cmd == "+CMTI"){
      //get newly arrived memory location and store it in temp
      index = buff.indexOf(",");
      String temp = buff.substring(index+1, buff.length()); 
      temp = "AT+CMGR=" + temp + "\r"; 
      //get the message stored at memory location "temp"
      serial.println(temp); 
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
  if(msg == "Safe_Mode"){  
    digitalWrite(ch1, LOW);
    digitalWrite(ch2, HIGH);
    digitalWrite(ch3, HIGH);
    digitalWrite(ch4, LOW);
    Reply("Safe_Mode Activated");
    p=1;
  }
  else if(msg == "Night_Mode"){
    digitalWrite(ch1, HIGH);
    digitalWrite(ch2, HIGH);
    digitalWrite(ch3, LOW);
    digitalWrite(ch4, LOW);
    Reply("Night_Mode Activated");
    p=2;
  }
  else if(msg == "Morning_Mode"){
    digitalWrite(ch1, LOW);
    digitalWrite(ch2, LOW);
    digitalWrite(ch3, HIGH);
    digitalWrite(ch4, HIGH);
    Reply("Morning_Mode Activated");
    p=3;
  }
  else if(msg == "Def_Mode"){
    digitalWrite(ch1, LOW);
    digitalWrite(ch2, LOW);
    digitalWrite(ch3, LOW);
    digitalWrite(ch4, LOW);
    Reply("Def_Mode Activated");
    p=4;
  }

  
  smsStatus = "";
  senderNumber="";
  receivedDate="";
  msg="";  
}

void Reply(String text)
{
    serial.print("AT+CMGF=1\r");
    delay(1000);
    serial.print("AT+CMGS=\""+PHONE+"\"\r");
    delay(1000);
    serial.print(text);
    delay(100);
    serial.write(0x1A); //ascii code for ctrl-26 //sim800.println((char)26); //ascii code for ctrl-26
    delay(1000);
    Serial.println("SMS Sent Successfully.");
}







  void getTime(){
      myRTC.updateTime();
      //penulisan data pada serial monitor komputer
      Serial.print("Current Date / Time: ");
      //fungsi penulisan data untuk tanggal
      Serial.print(myRTC.dayofmonth);
      //penulisan data "/" sebagai separator
      Serial.print("/");
      //fungsi penulisan data untuk bulan
      Serial.print(myRTC.month);
      //penulisan data "/" sebagai separator
      Serial.print("/");
      //fungsi penulisan data untuk tahun
      Serial.print(myRTC.year);
      //penulisan data untuk jarak
      Serial.print(" ");
      //fungsi penulisan data untuk jam
      Serial.print(myRTC.hours);
      Serial.print(":");
      //fungsi penulisan data untuk menit
      Serial.print(myRTC.minutes);
      Serial.print(":");
      //fungsi penulisan data untuk detik
      Serial.println(myRTC.seconds);
      delay(1000);
      
    }


void GetLDR(){
  dataLDR = analogRead(sldr);
  Serial.print("Nilai LDR: ");
  Serial.println(dataLDR);
  delay(1000);
  }


uint8_t getAmp(){
  I = sensor.getCurrentDC();
  
  if (I < 0.00){
    I= 0.00;
    }
  return I;
  }

  
uint8_t ShowSerialData()
{
  while(serial.available()!=0)
  Serial.write(serial.read());
  delay(1000); 
  
}

uint8_t pollSms()
{
 delay(500);
  while (Serial.available()) 
  {
    serial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(serial.available()) 
  {
    Serial.write(serial.read());//Forward what Software Serial received to Serial Port
  }
}
