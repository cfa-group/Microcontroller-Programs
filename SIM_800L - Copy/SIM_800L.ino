#include <SoftwareSerial.h>
#include <OneWire.h>
//#include <DHT.h>
//#include <LiquidCrystal_I2C.h>
//
//#define DHTPIN1 3 //pin DATA konek ke pin 3 Arduino
//#define DHTPIN2 4 //pin DATA konek ke pin 4 Arduino
//#define DHTTYPE DHT11 //tipe sensor DHT11
//#define BUZ 2 // BUZZER 
//#define MQ A0 // MQ-Sensor
//#define F1 A1 // Flame Sensor 1
//#define F2 A2 // Flame Sensor 2

//DHT dht1(DHTPIN1, DHTTYPE); //set sensor + koneksi pin sensor 1
//DHT dht2(DHTPIN2, DHTTYPE); //set sensor + koneksi pin sensor 2
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
SoftwareSerial mySerial(10, 11); //SIM900 Tx & Rx is connected to Arduino #7 & #8

//deklarasi variabel
//int humi1, temp1, humi2, temp2, f1, f2, mq; 


void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600); //baud 9600
//  pinMode(BUZ, OUTPUT);
//  digitalWrite(BUZ, LOW);
//  delay(1000); 
//  digitalWrite(BUZ, HIGH);
//  lcd.begin(20, 4);
//  lcd.setCursor(8, 0);
//  lcd.print("IoT");
//  lcd.setCursor(5, 1);
//  lcd.print("KEBAKARAN");
//  lcd.setCursor(0, 2);
//  lcd.print("MQSensor");
//  lcd.setCursor(9, 2);
//  lcd.print("FlameSensor");
//  lcd.setCursor(0, 3);
//  lcd.print("DHTSensor");
//  lcd.setCursor(14, 3);
//  lcd.print("SIM900");
//  delay(5000);
//  lcd.clear();
//  lcd.setCursor(0, 0);
//  lcd.print("MQ: ");
//  lcd.setCursor(17, 0);
//  lcd.print("PPM");
//  lcd.setCursor(0, 1);
//  lcd.print("F1: ");
//  lcd.setCursor(10, 1);
//  lcd.print("F2: ");
//  lcd.setCursor(0, 2);
//  lcd.print("T1: ");
//  lcd.setCursor(10, 2);
//  lcd.print("T2: ");
//  lcd.setCursor(0, 3);
//  lcd.print("H1: ");
//  lcd.setCursor(10, 3);
//  lcd.print("H2: "); 
}
 
void loop()
{
//  mq = analogRead(MQ);
//  f1 = analogRead(F1);
//  f2 = analogRead(F2);
//  humi1 = dht1.readHumidity();//baca kelembaban1
//  temp1 = dht1.readTemperature();//baca suhu1
//  humi2 = dht2.readHumidity();//baca kelembaban2
//  temp2 = dht2.readTemperature();//baca suhu2
//  if (isnan(humi1) || isnan(temp1)) { //jika tidak ada hasil
//    Serial.println("DHT11 tidak terbaca... !");
//    return;
//  }
//  else{//jika ada hasilnya 
//  Serial.print("MQ= ");
//  Serial.print(mq, DEC);
//  Serial.println(" PPM");
//  Serial.print("F1= ");
//  Serial.print(f1);
//  Serial.print(" || ");
//  Serial.print("F2= ");
//  Serial.println(f2);
//  Serial.print("Temp1= ");  //kirim serial "Suhu"
//  Serial.print(temp1);     //kirim serial nilai suhu
//  Serial.println(" C");    //kirim serial "C" Celcius 
//  Serial.print("Humi1= ");  //kirim serial "Humi"
//  Serial.print(humi1);     //kirim serial nilai kelembaban
//  Serial.println(" %RH");  //kirim serial "%RH"
//  }
//  if (isnan(humi2) || isnan(temp2)) { //jika tidak ada hasil
//    Serial.println("DHT11 tidak terbaca... !");
//    return;
//  }
//  else{//jika ada hasilnya 
//  Serial.print("Suhu2= ");  //kirim serial "Suhu"
//  Serial.print(temp2);     //kirim serial nilai suhu
//  Serial.println(" C");    //kirim serial "C" Celcius 
//  Serial.print("Humi2= ");  //kirim serial "Humi"
//  Serial.print(humi2);     //kirim serial nilai kelembaban
//  Serial.println(" %RH");  //kirim serial "%RH"
//  lcd.setCursor(4, 0);
//  lcd.print(mq);
//  lcd.setCursor(4, 1);
//  lcd.print(f1);
//  lcd.setCursor(14, 1);
//  lcd.print(f2);
//  lcd.setCursor(4, 2);
//  lcd.print(temp1);
//  lcd.setCursor(14, 2);
//  lcd.print(temp2);
//  lcd.setCursor(4, 3);
//  lcd.print(humi1);
//  lcd.setCursor(14, 3);
//  lcd.print(humi2);
//  }
//  delay(2000);           

//  if (mq > 880 || f1 > 550 || f2 > 550 || temp1 > 38 || temp2 > 38){
//    digitalWrite(BUZ, LOW);
    SendMessage();
//    }else{
//      digitalWrite(BUZ, HIGH);
//      }
//  SendData();
}

void SendMessage()
{
//  Serial.println("I am in send");
  mySerial.println("AT"); //Sets the GSM Module in Text Mode
  ShowSerialData();
//  mySerial.println("ATD081234540680"); //Sets the GSM Module in Text Mode
//  ShowSerialData();
  mySerial.println("AT+CSMP=17.167,0,0"); //Sets the GSM Module in Text Mode
  ShowSerialData();
  mySerial.println("AT+CMGF=1"); //Sets the GSM Module in Text Mode
  ShowSerialData();
//  mySerial.println("AT+CMGS=\"+6281234540680\"\r"); //Sets the GSM Module in Text Mode
//  ShowSerialData();
//  mySerial.println("PING"); //Sets the GSM Module in Text Mode
//  ShowSerialData();
  
  
//  
//  
//  delay(500); // Delay of 1000 milli seconds or 1 second
//  mySerial.println("AT+CMGS=\"+6281234540680\"\r"); // Replace x with mobile number
//  ShowSerialData();
//  delay(500);
//  mySerial.println("Danger!!!, Please check the condition of your home");// The SMS text you want to send
//  ShowSerialData();
//  delay(100);
//  mySerial.println((char)26);// ASCII code of CTRL+Z
//  ShowSerialData();
//  delay(500);
}

//void SendData()
//{
//  if (mySerial.available())
//    Serial.write(mySerial.read());
//    
//    mySerial.println("AT");
//    delay(500);
//   
//    mySerial.println("AT+CPIN?");
//    delay(500);
//   
//    mySerial.println("AT+CREG?");
//    delay(500);
//   
//    mySerial.println("AT+CGATT?");
//    delay(500);
//   
//    mySerial.println("AT+CIPSHUT");
//    delay(500);
//   
//    mySerial.println("AT+CIPSTATUS");
//    delay(500);
//   
//    mySerial.println("AT+CIPMUX=0");
//    delay(500);
//   
//    ShowSerialData();
//   
//    mySerial.println("AT+CSTT=\"internet\"");//start task and setting the APN,
//    delay(1000);
//   
//    ShowSerialData();
//   
//    mySerial.println("AT+CIICR");//bring up wireless connection
//    delay(1000);
//   
//    ShowSerialData();
//   
//    mySerial.println("AT+CIFSR");//get local IP adress
//    delay(1000);
//   
//    ShowSerialData();
//   
//    mySerial.println("AT+CIPSPRT=0");
//    delay(1000);
//   
//    ShowSerialData();
//    
//    mySerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
//    delay(2000);
//   
//    ShowSerialData();
//   
//    mySerial.println("AT+CIPSEND");//begin send data to remote server
//    delay(1000);
//    ShowSerialData();
//    
//    String str="GET https://api.thingspeak.com/update?api_key=6T8Z1VZ7B4X3W0S7&field1="+String(temp1)+"&field2="+String(temp2)+"&field3="+String(humi1)+"&field4="+String(humi2)+"&field5="+String(f1)+"&field6="+String(f2)+"&field7="+String(mq);
//    Serial.println(str);
//    mySerial.println(str);//begin send data to remote server
//    
//    delay(2000);
//    ShowSerialData();
//   
//    mySerial.println((char)26);//sending
//    delay(500);//waitting for reply, important! the time is base on the condition of internet 
//    mySerial.println();
//   
//    ShowSerialData();
//   
//    mySerial.println("AT+CIPSHUT");//close the connection
//    delay(100);
//    ShowSerialData();
//}
  
void ShowSerialData()
{
  while(mySerial.available()!=0)
  Serial.write(mySerial.read());
  delay(1000); 
  
}
