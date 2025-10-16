
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//


#define c1  A0
#define c2  A1
#define c3  A2
#define c4  A3
float d1, d2, d3, d4;
unsigned long  interval = 1500;
unsigned long previousMillis = 0;
int buzz = 2;

bool stat = false;

void setup() {
  // put your setup code here, to run once:

  pinMode(buzz, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pengujian GolDar");
  lcd.setCursor(0,1);
  lcd.print("Nurjannah");
//  
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  digitalWrite(13, HIGH);
  delay (2000);
  lcd.clear();
  
}

void loop() {
  // put your main code here, to run repeatedly:


    getData();
    Serial.println(d1);
    Serial.println(d2);
    Serial.println(d3);
    Serial.println(d4);
    Serial.println("----------------------------------");

    if (d1 > 800 && d2 > 800 && d3 > 800 && d4 > 800 && stat == false){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No-Card...");
      lcd.setCursor(0, 1);
      lcd.print("Insert_Card...");
      }else{
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Processing...");
        stat = true;
      }

//=======Penetapan parameter tiap sensor untuk pembacaan data golongan darah AB=========//
      
      if (d1 > 568 
       && d1 < 584 
       
       && d2 > 590
       && d2 < 605
       
       && d3 > 638
       && d3 < 650
       
       && d4 > 115
       && d4 < 135
       
       && stat == true
       ){
          infAB();
          delay(3500);
          stat = false;
          lcd.clear();
      }

//=======Penetapan parameter tiap sensor untuk pembacaan data golongan darah A=========//
      
      if(d1 > 570 
      && d1 < 582 
          
      && d2 > 500 
      && d2 < 599 
          
      && d3 > 500 
      && d3 < 599

      && d4 > 50
      && d4 < 120
          
      && stat == true
          
      ){
          infA();
          delay(3500);
          stat = false;
          lcd.clear();
    }

//=======Penetapan parameter tiap sensor untuk pembacaan data golongan darah B=========//
    
      if(d1 > 508 
      && d1 < 532 
      
      && d2 > 517
      && d2 < 550
      
      && d3 > 620
      && d3 < 655
      
      && d4 > 50 
      && d4 < 70 
      
      && stat == true
      ){
              infB();
              delay(3500);
              stat = false;
              lcd.clear();
      }

//=======Penetapan parameter tiap sensor untuk pembacaan data golongan darah O=========//
      
      if(d1 > 535 
      && d1 < 582
       
      && d2 > 445 
      && d2 < 510 
      
      && d3 > 570 
      && d3 < 600

      && d4 > 70 
      && d4 < 90

      && stat == true
      ){
                infO();
                delay(3500);
                stat = false;
                lcd.clear();
      }


      
//            else{
//              infErr();
//              delay(3500);
//              stat = false;
//              lcd.clear();
//              }
      


  delay(500);




}



uint8_t infAB (){
  if (stat == true){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Detect : AB");
    lcd.setCursor(0, 1);
    lcd.print("Rh : +");
    unsigned char i, j ;

        for (i = 0; i <80; i++) // When a frequency sound
        {
          digitalWrite (buzz, HIGH) ; //send tone
          delay (1) ;
          digitalWrite (buzz, LOW) ; //no tone
          delay (1) ;
        }
        for (j = 0; j <100; j++) 
        {
          digitalWrite (buzz, HIGH) ;
          delay (2) ;
          digitalWrite (buzz, LOW) ;
          delay (2) ;
        }
        
    }
  }

uint8_t infA(){
  if (stat == true){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Detect : A");
    lcd.setCursor(0, 1);
    lcd.print("Rh : +");
    unsigned char i, j ;
      
        for (i = 0; i <80; i++) // When a frequency sound
        {
          digitalWrite (buzz, HIGH) ; //send tone
          delay (1) ;
          digitalWrite (buzz, LOW) ; //no tone
          delay (1) ;
        }
        for (j = 0; j <100; j++) 
        {
          digitalWrite (buzz, HIGH) ;
          delay (2) ;
          digitalWrite (buzz, LOW) ;
          delay (2) ;
        }
        
    }
  }

uint8_t infB(){
  if (stat == true){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Detect : B");
    lcd.setCursor(0, 1);
    lcd.print("Rh : +");
    unsigned char i, j ;

        for (i = 0; i <80; i++) // When a frequency sound
        {
          digitalWrite (buzz, HIGH) ; //send tone
          delay (1) ;
          digitalWrite (buzz, LOW) ; //no tone
          delay (1) ;
        }
        for (j = 0; j <100; j++) 
        {
          digitalWrite (buzz, HIGH) ;
          delay (2) ;
          digitalWrite (buzz, LOW) ;
          delay (2) ;
        }
        
    }
  }

uint8_t infO(){
  if (stat == true){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Detect : O");
    lcd.setCursor(0, 1);
    lcd.print("Rh : +");
    unsigned char i, j ;
    
        for (i = 0; i <80; i++) // When a frequency sound
        {
          digitalWrite (buzz, HIGH) ; //send tone
          delay (1) ;
          digitalWrite (buzz, LOW) ; //no tone
          delay (1) ;
        }
        for (j = 0; j <100; j++) 
        {
          digitalWrite (buzz, HIGH) ;
          delay (2) ;
          digitalWrite (buzz, LOW) ;
          delay (2) ;
        }
      
    }
  }

uint8_t infErr(){
  if (stat == true){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Detect : Err...");
    lcd.setCursor(0, 1);
    lcd.print("Repeat...");
    }
  }  

uint8_t getData(){
    d1 = analogRead(c1);
    d2 = analogRead(c2);
    d3 = analogRead(c3);
    d4 = analogRead(c4);
    
    return d1, d2, d3, d4;  
  }
