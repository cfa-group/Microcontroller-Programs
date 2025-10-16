#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define Switch D0
bool stat = false;
int dt = 0;
String data = "";


String token =  "1023";
const char* ssid="kopi jie";  //Client or Laptop Connected SSID Wifi
const char* password = "pesankopi";  //PASSWORD SSID



void setup() {
  // put your setup code here, to run once:
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
  pinMode(Switch, INPUT);
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  dt = digitalRead(Switch);
  
  Serial.print("Stat = "); if (dt >= 1 && stat == false){
    Serial.print("TRUE");Serial.println(dt);
    data = "1";
    Send(data, token);
    stat = true;
    }else if (dt <= 0 && stat == true){
      Serial.print("FALSE");Serial.println(dt);
      data = "0";
      Send(data, token);
      stat = false;
      }else{
        Serial.print("Waiting Your Command... -> ");Serial.println(dt);
        delay(1000);
        }

      
  delay (100);

    
    

}




void Send(String data, String token){
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, "https://warkopdamai.site/api/getApi1?"); //HTTP
    http.addHeader("Content-Type", "application/json");

    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    int httpCode = http.POST("data="+data+"&token"+token);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  
  delay(100);
  
}
