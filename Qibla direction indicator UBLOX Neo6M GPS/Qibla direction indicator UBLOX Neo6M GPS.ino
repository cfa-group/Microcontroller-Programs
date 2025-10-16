//Pin GND GPS NEO-6M -> pin GND Arduino UNO
//Pin Rx  GPS NEO-6M -> pin D3 Arduino UNO
//Pin Tx  GPS NEO-6M -> pin D4 Arduino UNO
//Pin VCC GPS NEO-6M -> pin 5V Arduino UNO

//Pin SDA QMC5883 -> pin A4 Arduino UNO
//Pin SCL QMC5883 -> pin A5 Arduino UNO
//Pin GND QMC5883 -> pin GND Arduino UNO
//Pin VCC QMC5883 -> pin 5V Arduino UNO

//Pin (+) Buzzer -> Pin D7 Arduino UNO
//Pin (-) Buzzer -> Pin GND Arduino UNO

//Calling Library
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <DFRobot_QMC5883.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The Library object's
TinyGPSPlus gps;
DFRobot_QMC5883 compass;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

#define buzz 7
int Reset = 10;

float xv, yv, zv, declinationAngle, heading, headingDegrees, Amax, Amin, LT, BT, LK, BK, radLT, radBT, radLK, radBK, radSL, radSB;
int head, degg, a, amin, amax;
double atan_B, B, v, w, x, y, z, A;

float calibrated_values[3];
   
void transformation(float uncalibrated_values[3])    
{
      double calibration_matrix[3][3] = 
      {
        { 1.103,  -0.105,  -0.038},
        { 0.038,    1.09,  -0.003},
        {-0.005,       0,   1.138}  
      };
      double bias[3] = 
      {
        1697.201,
        1001.918,
        2443.618
      };  
      for (int i=0; i<3; ++i) uncalibrated_values[i] = uncalibrated_values[i] - bias[i];
      float result[3] = {0, 0, 0};
      for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j)
          result[i] += calibration_matrix[i][j] * uncalibrated_values[j];
      for (int i=0; i<3; ++i) calibrated_values[i] = result[i];
}

float scaler;
boolean scaler_flag = false;
float normal_vector_length;

void vector_length_stabilasation(){
      if (scaler_flag == false)
      {
        getHeading();
        normal_vector_length = sqrt(calibrated_values[0]*calibrated_values[0] + calibrated_values[1]*calibrated_values[1] + calibrated_values[2]*calibrated_values[2]);
        scaler_flag = true;
      } 
      //calculate the current scaler
      scaler = normal_vector_length/sqrt(calibrated_values[0]*calibrated_values[0] + calibrated_values[1]*calibrated_values[1] + calibrated_values[2]*calibrated_values[2]);
      //apply the current scaler to the calibrated coordinates (global array calibrated_values)
      calibrated_values[0] = calibrated_values[0]*scaler;
      calibrated_values[1] = calibrated_values[1]*scaler;
      calibrated_values[2] = calibrated_values[2]*scaler;
}

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  ss.begin(GPSBaud);
  Wire.begin(); 
  pinMode(buzz, OUTPUT);
  pinMode(Reset, OUTPUT);
  digitalWrite(Reset, LOW);
  //--------------------------------QMC5883-----------------------------------//
  while (!compass.begin()){                                                   
            Serial.println("Could not find a valid QMC5883 sensor, check wiring!"); 
            delay(500);                                                             
      }                                                                           
      compass.setRange(QMC5883_RANGE_2GA);                        
      compass.setMeasurementMode(QMC5883_CONTINOUS);                     
      compass.setDataRate(QMC5883_DATARATE_50HZ);                       
      compass.setSamples(QMC5883_SAMPLES_8);
      digitalWrite(buzz, HIGH);
      delay(50);
      digitalWrite(buzz, LOW);
      delay(100);
      digitalWrite(buzz, HIGH);
      delay(300);
      digitalWrite(buzz, LOW);

}



void loop() {
  // put your main code here, to run repeatedly:
    GPS();
}

void getHeading()
{ 
    //-------------------QMC5883----------------//
    Vector raw = compass.readRaw();
    xv = (float)raw.XAxis;
    yv = (float)raw.YAxis;
    zv = (float)raw.ZAxis;
}

int Compass(int degg){
  //--------------------------------QMC5883-----------------------------------//
        float values_from_magnetometer[3];
        getHeading();                             
        values_from_magnetometer[0] = xv;
        values_from_magnetometer[1] = yv;
        values_from_magnetometer[2] = zv;
        transformation(values_from_magnetometer);
        vector_length_stabilasation();
        heading = atan2(calibrated_values[0], -calibrated_values[1]);
        float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / PI);
        heading += declinationAngle;
        if (heading < 0){
            heading += 2 * PI;
        }
        if (heading > 2 * PI){
            heading -= 2 * PI;
        }
        headingDegrees = heading * 180/PI; 
        degg = floor(headingDegrees);
        
        Serial.print("Degress = ");
        Serial.print(degg);
        Serial.println("°");
        
        if (degg >= amin && degg <= amax){
        digitalWrite(buzz, HIGH);
        delay(30);
        digitalWrite(buzz, LOW);
        }else{
          digitalWrite(buzz, LOW);
          }
        delay(100);
        return Compass(degg);
  }

void GPS(){
 //--------------------------------GPS_NEO-6M---------------------------------//
  while(ss.available() > 0){
    gps.encode(ss.read());
      if (gps.location.isUpdated()){
        LT = gps.location.lat();
        BT = gps.location.lng();
        LK = 21.422500;
        BK = 39.826194;
  
        Serial.print("Get Data Coordinates: ");
        Serial.print("LT| "); 
        Serial.println(LT);
        Serial.print("----------------------BT| "); 
        Serial.println(BT);
        Serial.print("----------------------LK| "); 
        Serial.println(LK);
        Serial.print("----------------------BK| "); 
        Serial.println(BK);
        Serial.println("|----------------------------------------------------------------------|");
        Serial.println("|----------------------------------------------------------------------|");
        
        radLT = radians(LT);
        radBT = radians(BT);
        radLK = radians(LK);
        radBK = radians(BK);
        radSL = radLT-radLK;
        radSB = radBT-radBK;
        y = sin(radSB);
        v = cos(radLT)*tan(radLK);
        w = sin(radLT)*cos(radSB);
        x = v-w;
        z = y/x;
        atan_B = atan(z);
        B = degrees(atan_B);
        A = 360 - B;
        a = floor(A);
        Amax = A + 7;
        amax = floor(Amax);
        Amin = A - 7;
        amin = floor(Amin);
        
        Serial.print("Get Data Radians : ");
        Serial.print("radLT| "); 
        Serial.println(radLT);
        Serial.print("-------------------radBT| "); 
        Serial.println(radBT);
        Serial.print("-------------------radLK| "); 
        Serial.println(radLK);
        Serial.print("-------------------radBK| "); 
        Serial.println(radBK);
        Serial.println("Get Difference : ");
        Serial.print("-------------------radSL| "); 
        Serial.println(radSL);
        Serial.print("-------------------radSB| "); 
        Serial.println(radSB);
        Serial.println("|----------------------------------------------------------------------|");
        Serial.println("|----------------------------------------------------------------------|");
        
        Serial.print("(sin(radSB))______________________________________________= "); 
        Serial.println(y);
        Serial.print("(cos(radLT)*tan(radLK))___________________________________= "); 
        Serial.println(v);
        Serial.print("(sin(radLT)*cos(radSB))___________________________________= "); 
        Serial.println(w);
        Serial.print("(cos(radLT)*tan(radLK))-(sin(radLT)*cos(radSB))___________= "); 
        Serial.println(x);
        Serial.print("(sin(radSB))/(cos(radLT)*tan(radLK)-sin(radLT)*cos(radSB))= "); 
        Serial.println(z);
        Serial.print("Arc tan(B)________________________________________________= "); 
        Serial.println(atan_B);
        Serial.print("Degrees B_________________________________________________= "); 
        Serial.print(B);
        Serial.println("°");
        Serial.print("Degrees A_________________________________________________= "); 
        Serial.print(a);
        Serial.println("°");
        if (a > 360){
          return GPS();
          }
        Serial.print("Degrees Amax______________________________________________= "); 
        Serial.print(amax);
        Serial.println("°");
        Serial.print("Degrees Amin______________________________________________= ");
        Serial.print(amin);
        Serial.println("°");
        Serial.println("|----------------------------------------------------------------------|");
        Serial.println("|----------------------------------------------------------------------|");
        
        Serial.println("");
        Serial.println("");
  
        digitalWrite(buzz, HIGH);
        delay(50);
        digitalWrite(buzz, LOW);
        delay(100);
        digitalWrite(buzz, HIGH);
        delay(300);
        digitalWrite(buzz, LOW);
        delay(50);
        digitalWrite(buzz, HIGH);
        delay(500);
        digitalWrite(buzz, LOW);
        delay(1000);
        
        Compass(degg);
        
      }else{
        digitalWrite(buzz, LOW); 
        return GPS();
      } 
    }
}
 
