
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//
LiquidCrystal_I2C lcd(0x27, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
//


#define c1  A0
#define c2  A1
#define c3  A2
#define c4  A3
float d1, d2, d3, d4, Av, Avg;

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
  lcd.print("--My Name--");
//  
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  digitalWrite(13, HIGH);
  delay (2000);
  lcd.clear();
  
}

void loop() {
  // put your main code here, to run repeatedly:

  cekStat();
  
  if (d1 > 800 && d2 > 800 && d3 > 800 && d4 > 800 && stat == false){
    
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("No-Card...");
      lcd.setCursor(0, 1);
      lcd.print("Insert_Card...");
      stat = false;
      delay(1000);
      }else{
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Processing...");
        stat = true;
        int maxAvgPow = 3;
        float AvgPow = 0;
    
        for (int i = 0; i < maxAvgPow; i++){
            getData();
            Serial.println("|----------------------------------|");
            Serial.print("Count_AVGP: --> ");Serial.println(i);
            Serial.print("SumAll : ");Serial.println(Av);
            Serial.print("AvgAll ~ ");Serial.println(Avg);
            AvgPow += Avg;
            Serial.print("AVG_POW: ");Serial.println(AvgPow);
            Serial.println("|----------------------------------|");
            Serial.println("");
            
          }
          
            float Total = AvgPow/3;
            Serial.print("AVG_Total: ");Serial.println(Total);
            Serial.println("|----------------------------------|");
            Serial.println("");
            Serial.println("");
            Serial.println("");
          
          if(Total >= 4610 && Total <= 4965 && stat == true){
              infAB();
              delay(3500);
              stat = false;
              lcd.clear();
            }
            if(Total >= 4100 && Total <= 4250 && stat == true){
                infA();
                delay(3500);
                stat = false;
                lcd.clear();
              }
              if(Total >= 4323 && Total <= 4355 && stat == true){
                  infB();
                  delay(3500);
                  stat = false;
                  lcd.clear();
                }
                if(Total >= 4251 && Total <= 4322 && stat == true){
                    infO();
                    delay(3500);
                    stat = false;
                    lcd.clear();
                  }
      
      
          delay(500);
    
            
          }


}



int cekStat(){
    d1 = analogRead(c1);
    d2 = analogRead(c2);
    d3 = analogRead(c3);
    d4 = analogRead(c4);
  return d1, d2, d3, d4;
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
    
    int MaxCount = 10;
        float sum1 = 0;
        float sum2 = 0;
        float sum3 = 0;
        float sum4 = 0;
    d1 = analogRead(c1);
    d2 = analogRead(c2);
    d3 = analogRead(c3);
    d4 = analogRead(c4);
    
    for(int i = 0; i < MaxCount; i++){
        
      
       Serial.print("Count : ------> ");Serial.println(i);
       sum1 += d1;
       Serial.print("A1 = ");Serial.println(sum1);
       sum2 += d2;
       Serial.print("A2 = ");Serial.println(sum2);
       sum3 += d3;
       Serial.print("A3 = ");Serial.println(sum3);
       sum4 += d4;
       Serial.print("A4 = ");Serial.println(sum4);
       Serial.println("------------------");
       delay(300);
       
      }
    Av = sum1+sum2+sum3+sum4;
    Avg = Av / 4;
    return Av, Avg;  
  }
