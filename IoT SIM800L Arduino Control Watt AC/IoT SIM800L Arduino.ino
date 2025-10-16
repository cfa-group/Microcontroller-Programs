/* This code works with SIM800L Evb version, DHT11 and MLX90614
 * It sets the module on receiving SMS mode, wait if the user has sent a SMS containing keywords "DHT" or "MLX"
 * Check which one is received then proceed to get data from the chosen sensor and send it via SMS to the programmed phone number
 * And come back to receiving mode.
 * Refer to www.SurtrTech.com for more detaims
 */

#include <SoftwareSerial.h>        //Libraries required for Serial communication, i²c communication, DHT11 and MLX90614
#include <Wire.h>
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include <RTClib.h>
#include "ACS712.h"


RTC_DS3231 RTC;


#define ch1  7
#define ch2  6
#define ch3  5
#define ch4  4



ACS712 sensor(ACS712_30A, A1);
float U,
      I,
      P;

      

#define sldr A1
float dataLDR;
int Set = 0;
int p ;
float Mp;

const String all = "ALL";

char Received_SMS;              //Here we store the full received SMS (with phone sending number and date/time) as char
short LDR_OK=-1, //DATA_LDR
      CUR_OK=-1, //DATA_CURRENT_METER
      NLM_OK=-1, //NORMAL_MODE
      SFM_OK=-1, //SAFE_MODE
      MGM_OK=-1, //MORNING_MODE
      NTM_OK=-1  //NIGHT_MODE
      ;      //Used later it shows if there's the word "DHT"/"MLX" within the received SMS "-1" means they are not found

boolean SFM_STAT = false,
        NLM_STAT = false,
        MGM_STAT = false,
        NTM_STAT = false
        ;
      
String Data_SMS;               //Here's the SMS that we gonna send to the phone number, it may contain DHT data or MLX data


SoftwareSerial sim800l(2, 3);                     // RX,TX for Arduino and for the module it's TXD RXD, they should be inverted


//Adafruit_MLX90614 mlx = Adafruit_MLX90614();      //Declaring MLX instance and DHT
//DHT dht(DHTPIN, DHTTYPE);


void setup()
{
   
  sim800l.begin(9600);   //Begin all the communications needed Arduino with PC serial and Arduino with all devices (SIM800L+DHT+MLX)
  Serial.begin(9600);   
  RTC.begin();  
//  mlx.begin(); 
//  dht.begin();

//=======================Adjustment====================//
//  RTC.adjust(DateTime(__DATE__, __TIME__));
//  if (! RTC.isrunning()) {
//    Serial.println("RTC is NOT running!");
//    // following line sets the RTC to the date & time this sketch was compiled
//    RTC.adjust(DateTime(__DATE__, __TIME__));
//  }
//  const String delall = "DEL ALL";
    
  sim800l.println("AT");
  ShowSerialData();
  sim800l.println("AT+CMGD=25");
  ShowSerialData();
  sim800l.println("AT+CMGD=24");
  ShowSerialData();
  sim800l.println("AT+CMGD=23");
  ShowSerialData();
  sim800l.println("AT+CMGD=22");
  ShowSerialData();
  sim800l.println("AT+CMGD=21");
  ShowSerialData();
  sim800l.println("AT+CMGD=21,4");
  ShowSerialData();

  sim800l.println("AT+CMGR=25");
  ShowSerialData();
  sim800l.println("AT+CMGR=24");
  ShowSerialData();
  sim800l.println("AT+CMGR=23");
  ShowSerialData();
  sim800l.println("AT+CMGR=22");
  ShowSerialData();
  sim800l.println("AT+CMGR=21");
  ShowSerialData();

  delay(2000);
  sim800l.println("ATI");
  ShowSerialData();
  sim800l.println("AT+COPS?");
  ShowSerialData();
  sim800l.println("AT+CBC");
  ShowSerialData();
  sim800l.println("AT+CSQ");
  ShowSerialData();
  sim800l.println("AT+CCID");
  ShowSerialData();
  
  
 
  

  // Check whether it has registered on the network
//  sim800l.println("AT+CREG?");
//  ShowSerialData();
//  sim800l.println("AT+CMGF=1");
//  ShowSerialData();
//  sim800l.println("AT+CNMI=1,2,0,0,0");
//  ShowSerialData();
  
    
  sensor.calibrate();

  pinMode(ch1, OUTPUT); //Relay 1
  pinMode(ch2, OUTPUT); //Relay 2
  pinMode(ch3, OUTPUT); //Relay 1
  pinMode(ch4, OUTPUT); //Relay 2
  
  digitalWrite(ch1, HIGH);
  digitalWrite(ch2, HIGH);
  digitalWrite(ch3, HIGH);
  digitalWrite(ch4, HIGH);

  Serial.println("Starting ...");
  delay(3000);         //Delay to let the module connect to network, can be removed
  ReceiveMode();       //Calling the function that puts the SIM800L moduleon receiving SMS mode
  
}


uint8_t ShowSerialData()
{
  while(sim800l.available()!=0)
  Serial.write(sim800l.read());
  delay(1000); 
  
}


void loop() {
  
  String RSMS;             //We add this new variable String type, and we put it in loop so everytime gets initialized
                           //This is where we put the Received SMS, yes above there's Recevied_SMS variable, we use a trick below
                           //To concatenate the "char Recevied_SMS" to "String RSMS" which makes the "RSMS" contains the SMS received but as a String
                           //The recevied SMS cannot be stored directly as String
  
    while(sim800l.available()>0){       //When SIM800L sends something to the Arduino... problably the SMS received... if something else it's not a problem
        
        Received_SMS=sim800l.read();  //"char Received_SMS" is now containing the full SMS received
        Serial.print(Received_SMS);   //Show it on the serial monitor (optional)     
        RSMS.concat(Received_SMS);    //concatenate "char received_SMS" to RSMS which is "empty"

        //And this is why we changed from char to String, it's to be able to use this function "indexOf"
        //"indexOf function looks for the substring "x" within the String (here RSMS) and gives us its index or position
        //===================KEY========FUNCTION=================//
        LDR_OK=RSMS.indexOf("LDR");   //DATA_LDR
        CUR_OK=RSMS.indexOf("CUR");   //DATA_CURRENT_METER
        NTM_OK=RSMS.indexOf("NTM");   //NIGHT_MODE
        MGM_OK=RSMS.indexOf("MGM");   //MORNING_MODE
        SFM_OK=RSMS.indexOf("SFM");   //SAFE_MODE
        NLM_OK=RSMS.indexOf("NLM");   //NORMAL_MODE

        
                                      //For example if found at the beginning it will give "0" after 1 character it will be "1"
                                      //If it's not found it will give "-1", so the variables are integers
        
    }



    //====================================DATA_LDR=============================================
    
  if(LDR_OK!=-1){                         //If "DHT" word is found within the SMS, it means that DHT_OK have other value than -1 so we can proceed
    Serial.println("found LDR");          //Shows on the serial monitor "found DHT" (optional)
    dataLDR = analogRead(sldr);
    Mp = map(dataLDR, 0, 1023, 9, 99);
    Serial.print("Resistance: ");
    Serial.println(dataLDR);
    Serial.print("Percentage: ");
    Serial.print(Mp);
    Serial.println(" %");
    Serial.println();

    Data_SMS = "LDR\nVall = "+String(dataLDR,1)+" Ohm"+" \n%age = "+String(Mp,1)+" %";       //Prepare the SMS to send, it contains some strings like "DHT" "Temperature"...
                                                                                         //And then the values read
    
    Send_Data();                      //This function set the sending SMS mode, prepare the phone number to which we gonna send, and send "Data_SMS" String
    ReceiveMode();                   //Come back to Receving SMS mode and wait for other SMS
    
    LDR_OK=-1; //DATA_LDR
    CUR_OK=-1; //DATA_CURRENT_METER
    NLM_OK=-1; //NORMAL_MODE
    SFM_OK=-1; //SAFE_MODE
    MGM_OK=-1; //MORNING_MODE
    NTM_OK=-1; //NIGHT_MODE
  }


    //====================================DATA_CURRENT_METER=============================================
    
  if(CUR_OK!=-1){                   //Same thing if the "MLX" word is found, Serial.print things are optionnal
    delay(1000);
    Serial.println("found CUR");
    U = 230;
    I = sensor.getCurrentAC();
    P = U * I;
  
    Serial.println(String("P = ") + P + " Watts");
    Serial.println();
    Serial.println();
  
    Data_SMS = "CUR\n(Pemakaian) = "+String(P,1)+" Watts"+" \nObject = "+String(I,1)+" Volts";
  
    Send_Data();
    ReceiveMode();
    
    LDR_OK=-1; //DATA_LDR
    CUR_OK=-1; //DATA_CURRENT_METER
    NLM_OK=-1; //NORMAL_MODE
    SFM_OK=-1; //SAFE_MODE
    MGM_OK=-1; //MORNING_MODE
    NTM_OK=-1; //NIGHT_MODE
  }

  //====================================NIGHT_MODE=============================================

  if(NTM_OK!=-1){                   //Same thing if the "MLX" word is found, Serial.print things are optionnal
    delay(1000);

    Serial.println("NIGHT_MODE_ACTIVATE");
    Serial.println();
    Serial.println();
  
    Data_SMS = "\nNIGHT_MODE_ACTIVATE";

    SFM_STAT = false;
    NLM_STAT = false;
    MGM_STAT = false;
    NTM_STAT = true;

    Send_Data();
    ReceiveMode();

    LDR_OK=-1; //DATA_LDR
    CUR_OK=-1; //DATA_CURRENT_METER
    NLM_OK=-1; //NORMAL_MODE
    SFM_OK=-1; //SAFE_MODE
    MGM_OK=-1; //MORNING_MODE
    NTM_OK=-1; //NIGHT_MODE
    
  }

  
  //====================================MORNING_MODE=============================================

  if(MGM_OK!=-1){                   //Same thing if the "MLX" word is found, Serial.print things are optionnal
    delay(1000);

    Serial.println("MORNING_MODE_ACTIVATE");
    Serial.println();
    Serial.println();
  
    Data_SMS = "\nMORNING_MODE_ACTIVATE";
    
    SFM_STAT = false;
    NLM_STAT = false;
    MGM_STAT = true;
    NTM_STAT = false;

    Send_Data();
    ReceiveMode();

    LDR_OK=-1; //DATA_LDR
    CUR_OK=-1; //DATA_CURRENT_METER
    NLM_OK=-1; //NORMAL_MODE
    SFM_OK=-1; //SAFE_MODE
    MGM_OK=-1; //MORNING_MODE
    NTM_OK=-1; //NIGHT_MODE

  }


  //====================================SAFE_MODE=============================================

  if(SFM_OK!=-1){                   //Same thing if the "MLX" word is found, Serial.print things are optionnal
    delay(1000);


    Serial.println("SAFE_MODE_ACTIVATE");
    Serial.println();
    Serial.println();
  
    Data_SMS = "\nSAFE_MODE_ACTIVATE";
    
    SFM_STAT = true;
    NLM_STAT = false;
    MGM_STAT = false;
    NTM_STAT = false;

    Send_Data();
    ReceiveMode();

    LDR_OK=-1; //DATA_LDR
    CUR_OK=-1; //DATA_CURRENT_METER
    NLM_OK=-1; //NORMAL_MODE
    SFM_OK=-1; //SAFE_MODE
    MGM_OK=-1; //MORNING_MODE
    NTM_OK=-1; //NIGHT_MODE

  }


  //====================================NORMAL_MODE=============================================

  if(NLM_OK!=-1){                   //Same thing if the "MLX" word is found, Serial.print things are optionnal
    delay(1000);


    Serial.println("NORMAL_MODE_ACTIVATE");
    Serial.println();
    Serial.println();
  
    Data_SMS = "\nNORMAL_MODE_ACTIVATE";
    
    SFM_STAT = false;
    NLM_STAT = true;
    MGM_STAT = false;
    NTM_STAT = false;

    Send_Data();
    ReceiveMode();

    LDR_OK=-1; //DATA_LDR
    CUR_OK=-1; //DATA_CURRENT_METER
    NLM_OK=-1; //NORMAL_MODE
    SFM_OK=-1; //SAFE_MODE
    MGM_OK=-1; //MORNING_MODE
    NTM_OK=-1; //NIGHT_MODE

  }


  if(SFM_STAT == 1){
      dataLDR = analogRead(sldr);
      Mp = map(dataLDR, 0, 1023, 9, 99);
      Serial.print("Resistance: ");
      Serial.println(dataLDR);
      Serial.print("Percentage: ");
      Serial.print(Mp);
      Serial.println(" %");
      Serial.println();
      if (Mp >= 50) {
        digitalWrite(ch1, HIGH);
        digitalWrite(ch2, HIGH);
        digitalWrite(ch3, HIGH);
        digitalWrite(ch4, LOW);   //LAMPU LUAR LOW
        }else{
          digitalWrite(ch1, HIGH);
          digitalWrite(ch2, HIGH);
          digitalWrite(ch3, HIGH);
          digitalWrite(ch4, HIGH);
          }
        
      delay(1000);
  }


  if(NLM_STAT == 1){
      
        digitalWrite(ch1, LOW);     //LAMPU DALAM LOW
        digitalWrite(ch2, LOW);     //FASILITAS SOKET PRIVASI/KAMAR LOW
        digitalWrite(ch3, LOW);     //FASILITAS SOKET TERBUKA/LUAR LOW
        digitalWrite(ch4, LOW);     //LAMPU LUAR LOW
        
       
  
  }

  if(MGM_STAT == 1){
      
        digitalWrite(ch1, HIGH);
        digitalWrite(ch2, HIGH);
        digitalWrite(ch3, LOW);     //FASILITAS SOKET TERBUKA/LUAR LOW
        digitalWrite(ch4, HIGH);
        
       
  
  }
  
  if(NTM_STAT == 1){
      
        digitalWrite(ch1, LOW);     //LAMPU DALAM LOW
        digitalWrite(ch2, LOW);     //FASILITAS SOKET PRIVASI/KAMAR LOW
        digitalWrite(ch3, HIGH);
        digitalWrite(ch4, LOW);     //LAMPU LUAR LOW
        
       
  
  }

}


void Serialcom() //This is used with ReceiveMode function, it's okay to use for tests with Serial monitor
{
  delay(500);
  while(Serial.available())                                                                      
  {
    sim800l.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(sim800l.available())                                                                      
  {
    Serial.write(sim800l.read());//Forward what Software Serial received to Serial Port
  }
}

void ReceiveMode(){       //Set the SIM800L Receive mode
  
  sim800l.println("AT"); //If everything is Okay it will show "OK" on the serial monitor
  Serialcom();
  sim800l.println("AT+CMGF=1"); // Configuring TEXT mode
  Serialcom();
  sim800l.println("AT+CNMI=2,2,0,0,0"); //Configure the SIM800L on how to manage the Received SMS... Check the SIM800L AT commands manual
  Serialcom();
}

void Send_Data()
{
  Serial.println("Sending Data...");     //Displays on the serial monitor...Optional
  sim800l.print("AT+CMGF=1\r");          //Set the module to SMS mode
  delay(100);
  sim800l.print("AT+CMGS=\"+6281234540680\"\r");  //Your phone number don't forget to include your country code example +212xxxxxxxxx"+6285394186403
  delay(500);  
  sim800l.print(Data_SMS);  //This string is sent as SMS
  delay(500);
  sim800l.print((char)26);//Required to tell the module that it can send the SMS
  delay(500);
  sim800l.println();
  Serial.println("Data Sent.");
  delay(500);

}





//=======================DATA SENSOR / MODEULE====================//

void GetLDR(){
  dataLDR = analogRead(sldr);
  Mp = map(dataLDR, 0, 1023, 9, 99);
  Serial.print("Resistance: ");
  Serial.println(dataLDR);
  Serial.print("Percentage: ");
  Serial.print(Mp);
  Serial.println(" %");
  Serial.println();
  delay(1000);
  int Mp = 0;
  }



void cur(){
  U = 230;
  I = sensor.getCurrentAC();
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
