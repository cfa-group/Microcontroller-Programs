#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Servo.h> 
Servo servo;

#define Sensor D0
#define max_tick 6500

int i, detik, tick, timer = 0;  
int sensor_count[4], sensor_state[4];  
float volume[4];  
int warning_time=60;  
long interval=1000;
long previousMillis=0; // millis() returns an unsigned long.

String token = "12345";
String vol, count;
const char* ssid="MI Note WY";  //Client or Laptop Connected SSID Wifi
const char* password = "wkynta01";  //PASSWORD SSID

void read_sensor(int pin) {  
  sensor_count[pin]++;  
  volume[pin] = float(100 * (max_tick - sensor_count[pin]) / max_tick);
  vol = String(volume[i]);
  count = String(sensor_count[i]);
    Serial.print("Count:"); 
    Serial.println(count); 
    Serial.print(" Volume:"); 
    Serial.println(vol);
    Send(vol, count);
}

void setup() {
  
  servo.attach(D2);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }


  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );
  pinMode(Sensor, INPUT);
  servo.write (0);
  delay (3000);
  servo.write (180);
  delay (3000);

}

char state = 0;

void loop() {

  unsigned long currentMillis = millis();  
  if(currentMillis - previousMillis > interval) {  
    detik++; 
  Serial.println(detik);  
    } 

     if (detik >= warning_time) { //  
      Serial.println("WARNING !!!!!!!"); 
      vol = "inf";
      count = "inf";
      Send(vol, count);
      
      } 
  
  tick++;  
  
  if (tick > 1000) {  
    timer++; 
    tick = 0; 
    Serial.println(timer);  
    read_sensor(0);  
  }  

  if (timer > 30)state = 1;

  //sensor 1 
  if (sensor_state[0] == 0 && !digitalRead(Sensor)) { 
    read_sensor(0); 
    sensor_state[0] = 1; 
    detik=0; 
  } 
  if (sensor_state[0] == 1 && digitalRead(Sensor)) { 
    sensor_state[0] = 0; 
  } 

  delay(100);
  
}


void Send(String vol, String count){
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("http://192.168.43.228/Drip-Monitoring-Control-Management/dashboard/apr?");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST("token=" + token + "&vol=" + vol + "&count=" + count); //post 2 parameter
    delay(200);
    if (httpResponseCode > 0) {
      String response = http.getString();
      char json[500];
      response.toCharArray(json, 500);
      StaticJsonDocument<200> doc;
      deserializeJson(doc, json);
      
      int vol = doc["vol"];
      int count = doc["count"];
      int target = doc["target"]; 
      Serial.print("Vol = ");
      Serial.println(vol);
      Serial.print("Count = ");
      Serial.println(count);
      Serial.print("Target = ");
      Serial.println(target);
      Serial.println();
      
      int val = map(target, 5, 80, 0, 180);
      servo.write(val);
      delay(200);
      
//      Serial.println(response); //menampilkan respon dari website
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }
    http.end();  //Free resources
  } else {
    Serial.println("Error in WiFi connection");
  }
  delay(100);
  
  }
