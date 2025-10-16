#define c1  A0
#define c2  A1
#define c3  A2
#define c4  A3
float d1, d2, d3, d4;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  digitalWrite(13, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:


    getData();
    Serial.println(d1);
    Serial.println(d2);
    Serial.println(d3);
    Serial.println(d4);
    Serial.println("----------------------------------");
    delay(750);
}



uint8_t getData(){
    d1 = analogRead(c1);
    d2 = analogRead(c2);
    d3 = analogRead(c3);
    d4 = analogRead(c4);
    
    return d1, d2, d3, d4;  
  }




  
