#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

const int sch[] = {A1, A2, A3, A4, A5};
int ValSch[] = {00, 00, 00, 00, 00};

int call[] = {000, 000, 000, 000, 000} ;
RF24 radio(7, 8); // CE, CSN
const uint64_t pipe = 0xE8E8F0F0E1LL; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  for(byte i = 0; i < 5; i++){
    pinMode(sch[i], INPUT);
    }
  radio.begin();
  radio.openWritingPipe(pipe);
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(byte i = 0; i < 5; i++){
    ValSch[i] = analogRead(sch[i]);
    if(ValSch[i] > 300){
      call[i] = ValSch[i];
      radio.write(call, 1);
      Serial.print("ValSch ");
      Serial.print(i);
      Serial.print(" = ");
      Serial.println(call[i]);
      Serial.print("Pipe Comunication : ");
      Serial.println("0xE8E8F0F0E1LL"); 
      }
      Serial.print("ValSch ");
      Serial.print(i);
      Serial.print(" = ");
      Serial.println(ValSch[i]);
    }
    delay(50);
}
