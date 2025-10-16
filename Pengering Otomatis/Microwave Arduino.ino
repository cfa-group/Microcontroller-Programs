
const int VIN = A0; // define the Arduino pin A0 as voltage input (V in)
const float VCC   = 4.90;// supply voltage
const int MODEL = 2;   // enter the model (see above list)
//----------------LIBRARY---------------------//
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <RBDdimmer.h>
#include "DHT.h"
#include <Robojax_AllegroACS_Current_Sensor.h>

Robojax_AllegroACS_Current_Sensor robojax(MODEL,VIN);

//----------------Rotary Enc Config--------------//
#define CLK  10
#define DT  9
#define SW  12
int counter;
int currentStateCLK;
int lastStateCLK;
int btn2state;
String currentDir ="";
unsigned long lastButtonPress = 0;

//----------------Buzz Config-------------------//
#define buzz  6
#define relay  7

//----------------DHT22 Config------------------//
#define DHT1_PIN  4     // for first DHT module
#define DHT2_PIN  8     // for 2nd DHT module
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht1(DHT1_PIN, DHTTYPE);//for first DHT module
DHT dht2(DHT2_PIN, DHTTYPE);// for 2nd DHT module
//float tmp;

//----------------LCD Config--------------------//

LiquidCrystal_I2C lcd(0x27, 20, 4);

//----------------Dimmer Config-----------------//
#define USE_SERIAL  Serial
#define outputPin  3
#define zerocross  2
dimmerLamp dimmer(outputPin);
int outVal = 0;
int co;
long count;



//----------------Current Config----------------//


const int analogIn = A0;
int mVperAmp = 66;          /*use 185 for ACS712 5A, use 100 for ACS712 20A, use 66 for ACS712 30A*/
                             
                             
                             
double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;
double waktu = 0;
double biaya = 0;
double totalBiaya = 0;
unsigned long waktuLama = 0;
unsigned long interval = 1000;

bool LoadData;

void setup() {
  // put your setup code here, to run once:
  LoadData = false;
  pinMode(analogIn,INPUT);
  
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  
  pinMode(buzz, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW); //tidak menghubung
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alat__Pengering_Ikan");
  lcd.setCursor(0, 1);
  lcd.print("__Berbasis Arduino__");
  digitalWrite(buzz, HIGH);
  delay(500);
  digitalWrite(buzz, LOW);
  delay(1000);
  dht1.begin();//for first DHT module
  dht2.begin();//for 2nd DHT module 
  lcd.clear();
  USE_SERIAL.begin(9600); 
  dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE)
  USE_SERIAL.println("Dimmer Program is starting...");
  USE_SERIAL.println("Set value");
  lcd.setCursor(0, 1);
  lcd.print("Start...");
  delay(2000);
  lcd.clear();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //------------------------------------------------------
if(LoadData != true){
//  getCost();
// Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter ++;
      if(counter > 9){
        counter = 9;
        }
    } else {
      // Encoder is rotating CW so increment
      counter --;
      if(counter < 3){
        counter = 3;
        }
    }


    Serial.print(" | Counter: ");
    Serial.println(counter);
    lcd.setCursor(0, 0);
    lcd.print("Set Time Frame: ");
    lcd.setCursor(16, 0);
    lcd.print(counter);
    lcd.setCursor(19, 0);
    lcd.print("H");
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;

  // Read the button state
  int btnState = digitalRead(SW);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed! func-direct(heater(counter))");
      delay(10);
      heater(counter); 
    }

    // Remember last button press event
    lastButtonPress = millis();
  }

  // Put in a slight delay to help debounce the reading
  delay(1);


}else{
      unsigned long curnow = millis();
      unsigned long xter = 0;
      unsigned long inter = count;
      
      if (curnow - xter <= inter){
        
          Voltage = getVPP();
          VRMS = (Voltage/2.0) *0.707;
          AmpsRMS = (VRMS * 1000)/mVperAmp;
          
          Serial.print(AmpsRMS);
          Serial.println(" Amps RMS");
          
          Serial.print(":: Conf_Time-Opr____:  ");
          Serial.print(inter/1000);
          Serial.println(" sec.");
          Serial.print(":: Current_Time-Opr_:  ");
          Serial.print((curnow - inter) / 1000);
          Serial.println(" sec.");
          
          Loadview("Co:", 0, 0, //Set-Display--------------------
                   "Vr:", 0, 1,
                   "Ar:", 0, 2, 
                   "V+:", 0, 3,
                   //Field II
                   "*C:", 12, 0, 
                   "*C:", 12, 1,
                   "P+:", 12, 2, 
                   "LD:", 12, 3);
                                //Set-DataDisplay-------------------
          lcd.setCursor(3, 0);
          lcd.print("0.4");      
          lcd.setCursor(3, 1);
          lcd.print(VRMS);        // pr get cost per Voltage
          lcd.setCursor(3, 2);      
          lcd.print(AmpsRMS);        // pr get cost per Amp
          lcd.setCursor(3, 3);
          lcd.print(Voltage);
                
          lcd.setCursor(15, 0);
          lcd.print(GetTemp("c", 1));      
          lcd.setCursor(15, 1);
          lcd.print(GetTemp("c", 2));     
          lcd.setCursor(15, 2);
          lcd.print(dimmer.getPower());
          lcd.print("%");
          lcd.setCursor(15, 3);
          lcd.print("True");  
          delay(2500); 
          
          }else if(curnow - xter >= inter){
            
              finish();
              
          }else{
            
            }
    }

}




float getVPP(){
  float result;
  int readValue; //value read from the sensor
  int maxValue = 0; // store max value here
  int minValue = 1024; // store min value here
  
  uint32_t start_time = millis();
  while((millis()-start_time) < 1000){   // For 1 sec.
      readValue = analogRead(analogIn);
      // see if you have a new maxValue
      if (readValue > maxValue){
          /*record the maximum sensor value*/
          maxValue = readValue;
          }
      if (readValue < minValue){
          /*record the maximum sensor value*/
          minValue = readValue;
          }
      }
  // Subtract min from max
  result = ((maxValue - minValue) * 5.0)/1024.0;
  
  return result;
}



float GetTemp(String req, int dhtCount){
  
  if(dhtCount ==1){
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h1 = dht1.readHumidity();
    // Read temperature as Celsius (the default)
    float t1 = dht1.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f1 = dht1.readTemperature(true);
  
    // Compute heat index in Fahrenheit (the default)
    float hif1 = dht1.computeHeatIndex(f1, h1);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic1 = dht1.computeHeatIndex(t1, h1, false);
  
    // Check if any reads failed and exit early (to try again).
    if (isnan(h1) || isnan(t1) || isnan(f1)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    
  
    // Compute heat index in Kelvin 
    float k1 = t1 + 273.15;
    if(req =="c"){
      return t1;//return Cilsus
    }else if(req =="f"){
      return f1;// return Fahrenheit
    }else if(req =="h"){
      return h1;// return humidity
    }else if(req =="hif"){
      return hif1;// return heat index in Fahrenheit
    }else if(req =="hic"){
      return hic1;// return heat index in Cilsus
    }else if(req =="k"){
      return k1;// return temprature in Kelvin
    }else{
      return 0.000;// if no reqest found, retun 0.000
    }
  }// DHT1 end
  
  
  if(dhtCount ==2){
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h2 = dht2.readHumidity();
    // Read temperature as Celsius (the default)
    float t2 = dht2.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f2 = dht2.readTemperature(true);
  
    // Compute heat index in Fahrenheit (the default)
    float hif2 = dht2.computeHeatIndex(f2, h2);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic2 = dht2.computeHeatIndex(t2, h2, false);
  
    // Check if any reads failed and exit early (to try again).
    if (isnan(h2) || isnan(t2) || isnan(f2)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    
  
    // Compute heat index in Kelvin 
    float k2 = t2 + 273.15;
    if(req =="c"){
      return t2;//return Cilsus
    }else if(req =="f"){
      return f2;// return Fahrenheit
    }else if(req =="h"){
      return h2;// return humidity
    }else if(req =="hif"){
      return hif2;// return heat index in Fahrenheit
    }else if(req =="hic"){
      return hic2;// return heat index in Cilsus
    }else if(req =="k"){
      return k2;// return temprature in Kelvin
    }else{
      return 0.000;// if no reqest found, retun 0.000
    }
  }
  // DHT2 end
}//------------------------------------------------------getTemp end
  
  

uint8_t heater(int co){
    int preVal = outVal;
    if (co > 2 && co < 10)
    {
      int buf = co;
      if (buf != 0){
        outVal = buf;
        }
      delay(100);

    }
    dimmer.setPower(map(outVal, 9, 3, 75, 95)); // setPower(0-100%);
    
    if (preVal != outVal)
    {
      digitalWrite(relay, HIGH);//------------------------Menghubungkan !!!
      digitalWrite(buzz, HIGH);
      delay(1000);
      digitalWrite(buzz, LOW); 
      delay(500);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set-TimeFrame : ");
      lcd.print(co);
      lcd.print("H");
      lcd.setCursor(0, 1);
      lcd.print("HeaterVal -> ");
      lcd.print(dimmer.getPower());
      lcd.print("%");
      LoadData = true; //-------------------------------------
      count = co*3600000;
      USE_SERIAL.print("______LoadData_STATEMENT == ");
      USE_SERIAL.println(LoadData);
      USE_SERIAL.print("______With Conf_Time-Opr == ");
      USE_SERIAL.print(count / 1000);
      USE_SERIAL.println(" Sec. Opr");
      USE_SERIAL.print("HeaterValue -> ");
      USE_SERIAL.print(dimmer.getPower());
      USE_SERIAL.println(" %");

      delay(1000);
      lcd.clear();
      return co, count;
    }
    delay(50);
  }

int LoadDataNull(){
    digitalWrite(relay, LOW);
      Loadview("Co:", 0, 0, 
             "rV:", 0, 1,
             "V_:", 0, 2, 
             "A_:", 0, 3,
             //Field II
             "Rp:", 9, 0, 
             "*C:", 9, 1,
             "P+:", 9, 2, 
             "LD:", 9, 3);
      //Field I
      lcd.setCursor(3, 0);
      lcd.print("_");      
      lcd.setCursor(3, 1);
      lcd.print("_");        
      lcd.setCursor(3, 2);
      lcd.print("_");      
      lcd.setCursor(3, 3);
      lcd.print("_");
      //Field II
      lcd.setCursor(12, 0);
      lcd.print("_  ");      
      lcd.setCursor(12, 1);
      lcd.print("_  ");    
      lcd.setCursor(12, 2);
      lcd.print("_  ");
      lcd.print("%");
      lcd.setCursor(12, 3);
      lcd.print("  False.");

      delay(5000);
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("LoadData_NULL...");
      lcd.setCursor(0, 2);
      lcd.print("Returning-Setup()..!");
      delay(1000);
      setup();
  }



void finish(){
  Serial.print("Finish....:)");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Finish...:)");
  lcd.setCursor(0, 1);
  lcd.print("Return Setup()...!");
  delay(2000);
  digitalWrite(relay, LOW);
  digitalWrite(buzz, HIGH);
  delay(2000);
  digitalWrite(buzz, LOW);
  delay(200);
  setup();
  }


//Base Load View Lcd20x4 i2c Func and Param 

uint8_t Loadview(String d1, int xf1, int xr1, //row1
                 String d2, int xf2, int xr2, //row2
                 String d3, int xf3, int xr3, //row3
                 String d4, int xf4, int xr4, //row4
                                            String d5, int xf5, int xr5, //row1
                                            String d6, int xf6, int xr6, //row2
                                            String d7, int xf7, int xr7, //row3
                                            String d8, int xf8, int xr8){//row4
                lcd.setCursor(xf1, xr1);
                lcd.print(d1);     
                lcd.setCursor(xf2, xr2);
                lcd.print(d2);    
                lcd.setCursor(xf3, xr3);
                lcd.print(d3);
                lcd.setCursor(xf4, xr4);
                lcd.print(d4);
                                            lcd.setCursor(xf5, xr5);
                                            lcd.print(d5);
                                            lcd.setCursor(xf6, xr6);
                                            lcd.print(d6);   
                                            lcd.setCursor(xf7, xr7);
                                            lcd.print(d7);                          
                                            lcd.setCursor(xf8, xr8);
                                            lcd.print(d8);
  }
