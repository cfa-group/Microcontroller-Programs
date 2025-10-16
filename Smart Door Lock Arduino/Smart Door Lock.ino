#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include<EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#define relay_pin 9
#define pinBuzz 11

char password[4];
char initial_password[4], new_password[4];

int i=0;
char key_pressed=0;
const byte rows = 4; 
const byte columns = 3; 
char hexaKeys[rows][columns] = {

  {'1','2','3'},
  
  {'4','5','6'},
  
  {'7','8','9'},
  
  {'*','0','#'}

};

byte row_pins[rows] = {5,4,3,2};
byte column_pins[columns] = {8,7,6};   
Keypad keypad_key = Keypad( makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

//----------------------------------------------------------------------------------------//

void setup()

{

  lcd.begin(20, 4);
  lcd.setCursor(2, 1);
  lcd.print("K");
  delay(800);
  lcd.setCursor(3, 1);
  lcd.print("E");
  delay(700);
  lcd.setCursor(4, 1);
  lcd.print("Y");
  delay(600);
  lcd.setCursor(5, 1);
  lcd.print("P");
  delay(500);
  lcd.setCursor(6, 1);
  lcd.print("A");
  delay(400);
  lcd.setCursor(7, 1);
  lcd.print("D");
  delay(300);
  lcd.setCursor(8, 1);
  lcd.print("_");
  delay(200);
  lcd.setCursor(9, 1);
  lcd.print("3");
  delay(100);
  lcd.setCursor(10, 1);
  lcd.print("X");
  delay(100);
  lcd.setCursor(11, 1);
  lcd.print("4");
  delay(100);
  lcd.setCursor(12, 1);
  lcd.print("_");
  delay(100);
  lcd.setCursor(13, 1);
  lcd.print("_");
  delay(70);
  lcd.setCursor(14, 1);
  lcd.print("D");
  delay(70);
  lcd.setCursor(15, 1);
  lcd.print("O");
  delay(70);
  lcd.setCursor(16, 1);
  lcd.print("O");
  delay(70);
  lcd.setCursor(17, 1);
  lcd.print("R");
  delay(70);
  lcd.setCursor(11, 2);
  lcd.print("K");
  delay(70);
  lcd.setCursor(10, 2);
  lcd.print("C");
  delay(70);
  lcd.setCursor(9, 2);
  lcd.print("O");
  delay(70);
  lcd.setCursor(8, 2);
  lcd.print("L");
  delay(3000);
  lcd.clear();

  delay(1000);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, HIGH);
  
  pinMode(pinBuzz, OUTPUT);
  beep(50);
  Serial.begin(9600);

  

}

//------------------------------------------------------------------------------------//

void loop()
{
  lcd.setCursor(5, 0);
  lcd.print("KEYPAD_3X4");
  lcd.setCursor(5, 1);
  lcd.print("DOOR--LOCK");
  lcd.setCursor(2, 2);
  lcd.print("INPUT-PASSWORD: ");

  initialpassword();
  
  digitalWrite(relay_pin, HIGH);
  key_pressed = keypad_key.getKey();
  
  if(key_pressed=='#')
    change();
  if (key_pressed)
  {
    
    lcd.setCursor(8, 3);
    lcd.print("*");
    beep(50);
    password[i++]=key_pressed;
    
      }
      
  if(i==4)
  {
    delay(200);
    for(int j=0;j<4;j++)
      initial_password[j]=EEPROM.read(j);
    if(!(strncmp(password, initial_password,4)))
    {
      Open();
      
    }
    
    else
    {
      wrong();
      
    }
    
  }
  
}

//----------------------------------------------------------------------------------------//

void change()
{
  int j=0;
  lcd.clear();
  lcd.print("Current Password");
  lcd.setCursor(0,1);
  while(j<4)
  {
    char key=keypad_key.getKey();
    if(key)//----------
    {//-----------
      beep(50);
      new_password[j++]=key;
      lcd.setCursor(8, 3);
      lcd.print("*");
    }//-----------
    key=0;//---------
  }
  delay(500);
  
  if((strncmp(new_password, initial_password, 4)))
  {
    wrong();
  }
  else
  {
    j=0;
    lcd.clear();
    lcd.print("New Password:");
    lcd.setCursor(0,1);//------------------
    while(j<4)
    {
      char key=keypad_key.getKey();
      if(key)
      {
        initial_password[j]=key;
        lcd.setCursor(8, 3);
        lcd.print("*");
        beep(50);
        EEPROM.write(j,key);
        j++;
      }
    }
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Pass Changed");
    delay(1000);
  }
  lcd.clear();
  key_pressed=0;
}

//-------------------------------------------------------------------------------------//

void initialpassword(){
  for(int j=0;j<4;j++)
    EEPROM.write(j, j+49);
  for(int j=0;j<4;j++)
    initial_password[j]=EEPROM.read(j);
} 

//-----------------------------------------------------------------------//

void beep(unsigned char delayms){
  analogWrite(11, 20);      // Almost any value can be used except 0 and 255
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(11, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}  

//----------------------------------------------------------------------------------//

void wrong(){
  // pass wrong!
  lcd.clear();
  lcd.setCursor(2, 3);
  lcd.print("Wrong Password:(");
  delay(1000);
  beep(100);
  delay(100);
  beep(150);
  delay(100);
  beep(200);
  lcd.clear();
  delay(1000);
  return;
  }

//--------------------------------------------------------------------------------//

void Open(){
  delay(2000);
  lcd.clear();
  lcd.setCursor(4, 3);
  lcd.print("Door is Open");
  digitalWrite(relay_pin, LOW);
  
  beep(50);
  delay(100);
  beep(100);
  delay(4000);
  
  digitalWrite(relay_pin, HIGH);
  lcd.clear();
  lcd.setCursor(3, 3);
  lcd.print("Door is Closed");
  delay(2000);
  lcd.clear();
  delay(1000);
  return;
}
