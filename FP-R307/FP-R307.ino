//https://www.adafruit.com/product/751
//https://github.com/johnrickman/LiquidCrystal_I2C

#include <Adafruit_Fingerprint.h>
#include <LiquidCrystal_I2C.h>
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal_I2C lcd(0x27, 20, 4);

void(* res) (void) = 0;
#define relay 7
#define bt 5
int tm;
int ver;
uint8_t id;

void setup()  
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  
  //----------------------------- Set I/O -----------------------
  pinMode(relay, OUTPUT);
  pinMode(bt, INPUT_PULLUP);
  digitalWrite(relay, HIGH);
  ver = 0;
  
  //----------------------------- test fp -----------------------
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");
  lcd.print("Serial Test-   Valid");
  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
    lcd.setCursor(0, 1);
    lcd.print("f.verPass-     Valid");
    delay(1000);
    lcd.setCursor(0, 2);
    lcd.print("PathSensor-    Valid");
    delay(3000);
    
    } else {
        Serial.println("Did not find fingerprint sensor :(");
        Serial.println("Please Check Path...");
        lcd.setCursor(1, 0);
        lcd.print("f.verPass-  notValid");
        delay(1000);
        lcd.setCursor(2, 0);
        lcd.print("PathSensor- notValid");
        while (1) { delay(1); }
      }

  //----------------------- Count Template fp -------------------
  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  tm = finger.templateCount + 1;
 
}

void loop()                     // run over and over again
{
  
  boolean b = digitalRead(bt);
  while (!b){
    Verify();
    }
  fp();  
}

int fp(){
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Finger Print");
  lcd.setCursor(6, 1);
  lcd.print("Scanning");
  lcd.setCursor(8, 2);
  lcd.print("....");
  getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
  }

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    //-----------------output if not match-----------------
    //not display
    //-----------------end output-------------------
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  if (ver == 1){
    Serial.println("Found ID #"); Serial.print(finger.fingerID); 
    Serial.println(" with confidence of "); Serial.println(finger.confidence);
    if (finger.fingerID != 1){
      Serial.println("  Sorry, verification doesn't belong to admin!!!");
      return finger.fingerID; 
      } else {
              Serial.println("   Enroll Open!");
              Serial.println("    Ready to enroll a fingerprint!");
              Serial.print("     Please type in the ID # "); Serial.print(tm); Serial.println(" you want to save this finger as...");
              id = readnumber();
              if (id < 2) {// ID #0 not allowed, try again!
                 Serial.println("      ID not valid :(");
                 return;
              }
              if (id > 25) {// ID #0 not allowed, try again!
                 Serial.println("      ID MAX :(");
                 return;
              }
              if (id != tm) {// ID #0 not allowed, try again!
                 Serial.println("      ID not valid :(");
                 return;
              }
              Serial.print("Enrolling ID #");
              Serial.println(id);
              
              while (!  getFingerprintEnroll() );
    }
  }
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
    opn();
  return finger.fingerID; 
}

int opn(){
    //Open
    lcd.setCursor(2, 3);
    lcd.print("O p e n . . .-");
    Serial.println("Relay Stt-High");
    delay(1000);
    digitalWrite(relay, LOW);
    delay(5000);
    lcd.clear();
    cls();
  }

int cls(){
      //Close
      lcd.setCursor(1, 3);
      lcd.print("C l o s e . . .-");
      Serial.println("Relay Stt-Low");
      digitalWrite(relay, HIGH);
      delay(1000);
      lcd.clear();
  }  

//-----------------------------verify after enroll------------------------
int Verify() {

  //-------------display lcd out
  delay(100);
  lcd.setCursor(0, 2);
  lcd.print("Admin Verification ");
  lcd.setCursor(5, 3);
  lcd.print("Please...!");
  delay(100);
  //-------------function verify
  ver = 1;
  getFingerprintID();
}

//---------------------------------------------Read Num------------------------------------
uint8_t readnumber(void) {
  uint8_t num = 0;
  
  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

//------------------------------------------------Enroll----------------------------------------
uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    Serial.println(" ----- Within 5 seconds it will reset automatically !");
    delay(1000);
    lcd.clear();
    lcd.print("Reset Execute in...");
    lcd.setCursor(7, 1);
    lcd.print("Sec.");
    int r = 5;
    while (r > 0){
      r--;
      lcd.setCursor(6, 1);
      lcd.print(r);
      delay(1000);
      return r;
      } 
    
    res();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}
