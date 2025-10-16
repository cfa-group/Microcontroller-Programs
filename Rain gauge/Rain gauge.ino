//Curah hujan adalah jumlah air yang jatuh di permukaan tanah selama periode tertentu yang diukur dengan satuan tinggi milimeter (mm) di atas permukaan horizontal.
//Curah hujan 1 mm adalah jumlah air hujan yang jatuh di permukaan per satuan luas (m2) dengan volume sebanyak 1 liter tanpa ada yang menguap, meresap atau mengalir.
// Lebih lengkap silahkan dipelajari lebih lanjut disini https://www.climate4life.info/2015/12/hujan-1-milimeter-yang-jatuh-di-jakarta.html

//Perhitungan rumus
//Tinggi curah hujan (cm) = volume yang dikumpulkan (mL) / area pengumpulan (cm2)
//Luas kolektor (Corong) 5,5cm x 3,5cm = 19,25 cm2
//Koleksi per ujung tip kami dapat dengan cara menuangkan 100ml air ke kolektor kemudian menghitung berapa kali air terbuang dari tip,
//Dalam perhitungan yang kami lakukan air terbuang sebanyak 70 kali. 100ml / 70= 1.42mL per tip.
//Jadi 1 tip bernilai 1.42 / 19.25 = 0,07cm atau 0.70 mm curah hujan.

// PENTING
// Nilai kalibrasi yang kami lakukan berlaku untuk semua sensor curah hujan yang kami jual tentu Anda dapat melakukan kalibrasi ulang sendiri jika dibutuhkan.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include "RTClib.h"
#include <virtuabotixRTC.h>   
virtuabotixRTC myRTC(6, 7, 8);
//RTC_DS1307 rtc;

// Gunakan pin D2 pada Arduino, Tegangan 5V Kemudian upload code ini
const int pin_interrupt = 2; // Menggunakan pin interrupt https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
long int jumlah_tip = 0;
float curah_hujan = 0.00;
float curah_hujan_per_menit = 0.00;
float curah_hujan_per_jam = 0.00;
float curah_hujan_per_hari = 0.00;
float curah_hujan_hari_ini = 0.00;
float temp_curah_hujan_per_menit = 0.00;
float temp_curah_hujan_per_jam = 0.00;
float temp_curah_hujan_per_hari = 0.00;
float milimeter_per_tip = 0.70;
String cuaca;

volatile boolean flag = false;

// Set alamat I2C LCD 0x27
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Inisialisasi struktur waktu
String jam, menit, detik;

void hitung_curah_hujan()
{
  flag = true;
}

void setup()
{
  Serial.begin(9600);
  // Inisialisasi LCD
  lcd.init();

  // Menyalakan lampu latar lCD
  lcd.backlight();
  //Clear LCD
  lcd.clear();

  pinMode(pin_interrupt, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin_interrupt), hitung_curah_hujan, FALLING); // Akan menghitung tip jika pin berlogika dari HIGH ke LOW
  // Inisialisasi RTC
//  if (!rtc.begin())
//  {
//    Serial.println("Couldn't find RTC");
//    while (1)
//      ;
//  }
  //=======Hanya dibuka komen nya jika akan kalibrasi waktu saja (hanya sekali) setelah itu harus di tutup komennya kembali supaya tidak set waktu terus menerus=======
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set waktu langsung dari waktu PC
  //rtc.adjust(DateTime(2021, 12, 2, 8, 57, 0)); // Set Tahun, bulan, tanggal, jam, menit, detik secara manual
  // Cukup dibuka salah satu dari 2 baris diatas, pilih set waktu secara manual atau dari PC
  //===================================================================================================================================================================
  lcd.setCursor(0, 2);
  lcd.print("/m=");
  lcd.print(curah_hujan_per_menit, 1);
  lcd.print(",/h=");
  lcd.print(curah_hujan_per_jam, 1);
  lcd.print(" ");
  lcd.setCursor(0, 3);
  lcd.print("/d=");
  lcd.print(curah_hujan_per_hari, 1);
  lcd.print(" mm ");
}

void loop()
{
  if (flag == true) // don't really need the == true but makes intent clear for new users
  {
    curah_hujan += milimeter_per_tip; // Akan bertambah nilainya saat tip penuh
    jumlah_tip++;
    delay(500);
    flag = false; // reset flag
  }



  myRTC.updateTime();                                                                                    //| 
                                                                                                         //| 
// Start printing elements as individuals                                                                //|   

  jam = String(myRTC.hours, DEC);
  menit = String(myRTC.minutes, DEC);
  detik = String(myRTC.seconds, DEC);
  Serial.print("Current Date / Time: ");                                                                 //| 
  Serial.print(myRTC.dayofmonth);                                                                        //| 
  Serial.print("/");                                                                                     //| 
  Serial.print(myRTC.month);                                                                             //| 
  Serial.print("/");                                                                                     //| 
  Serial.print(myRTC.year);                                                                              //| 
  Serial.print("  ");                                                                                    //| 
  Serial.print(jam);                                                                             //| 
  Serial.print(":");                                                                                     //| 
  Serial.print(menit);                                                                           //| 
  Serial.print(":");                                                                                     //| 
  Serial.println(detik); 

  
//  DateTime now = rtc.now(); // Ambil data waktu dari DS3231
//  jam = String(now.hour(), DEC);
//  menit = String(now.minute(), DEC);
//  detik = String(now.second(), DEC);
  curah_hujan_hari_ini = jumlah_tip * milimeter_per_tip;
  lcd.setCursor(0, 1);
  lcd.print(konversi(jam) + ":" + konversi(menit));
  lcd.print("->");
  lcd.print(jumlah_tip);
  lcd.print("x,");
  lcd.print(curah_hujan_hari_ini, 1);
  lcd.print(" ");
  temp_curah_hujan_per_menit = curah_hujan;

  //Probabilistik Curah Hujan https://www.bmkg.go.id/cuaca/probabilistik-curah-hujan.bmkg
  if (curah_hujan_hari_ini <= 0.00 && curah_hujan_hari_ini <= 0.50)
  {
    cuaca = "Berawan           ";
  }
  if (curah_hujan_hari_ini > 0.50 && curah_hujan_hari_ini <= 20.00)
  {
    cuaca = "Hujan Ringan      ";
  }
  if (curah_hujan_hari_ini > 20.00 && curah_hujan_hari_ini <= 50.00)
  {
    cuaca = "Hujan Sedang      ";
  }
  if (curah_hujan_hari_ini > 50.00 && curah_hujan_hari_ini <= 100.00)
  {
    cuaca = "Hujan Lebat       ";
  }
  if (curah_hujan_hari_ini > 100.00 && curah_hujan_hari_ini <= 150.00)
  {
    cuaca = "Hujan Sangat Lebat";
  }
  if (curah_hujan_hari_ini > 150.00)
  {
    cuaca = "Hujan ekstrem     ";
  }
  lcd.setCursor(0, 0);
  lcd.print(cuaca);      // Print cuaca hari ini (Ini bukan ramalan cuaca tapi membaca cuaca yang sudah terjadi/ sedang terjadi hari ini)
  if (detik.equals("0")) // Hanya print pada detik 0
  {
    curah_hujan_per_menit = temp_curah_hujan_per_menit; // Curah hujan per menit dihitung ketika detik 0
    temp_curah_hujan_per_jam += curah_hujan_per_menit;  // Curah hujan per jam dihitung dari penjumlahan curah hujan per menit namun disimpan dulu dalam variabel temp
    if (menit.equals("0"))
    {
      curah_hujan_per_jam = temp_curah_hujan_per_jam;   // Curah hujan per jam baru dihitung ketika menit 0
      temp_curah_hujan_per_hari += curah_hujan_per_jam; //// Curah hujan per hari dihitung dari penjumlahan curah hujan per jam namun disimpan dulu dalam variabel temp
      temp_curah_hujan_per_jam = 0.00;                  // Reset temp curah hujan per jam
    }
    if (menit.equals("0") && jam.equals("0"))
    {
      curah_hujan_per_hari = temp_curah_hujan_per_hari; // Curah hujan per jam baru dihitung ketika menit 0 dan jam 0 (Tengah malam)
      temp_curah_hujan_per_hari = 0.00;                 // Reset temp curah hujan per hari
      curah_hujan_hari_ini = 0.00;                      // Reset curah hujan hari ini
      jumlah_tip = 0;                                   // Jumlah tip di reset setap 24 jam sekali (Tengah malam)
    }
    lcd.setCursor(0, 2); // Menampilkan nilai pada LCD
    lcd.print("/m=");
    lcd.print(curah_hujan_per_menit, 1);
    lcd.print(",/h=");
    lcd.print(curah_hujan_per_jam, 1);
    lcd.print(" ");
    lcd.setCursor(0, 3);
    lcd.print("/d=");
    lcd.print(curah_hujan_per_hari, 1);
    lcd.print(" mm ");
    temp_curah_hujan_per_menit = 0.00;
    curah_hujan = 0.00;
    delay(1000);
  }
}

String konversi(String angka) // Fungsi untuk supaya jika angka satuan ditambah 0 di depannya, Misalkan jam 1 maka jadi 01 pada LCD
{
  if (angka.length() == 1)
  {
    angka = "0" + angka;
  }
  else
  {
    angka = angka;
  }
  return angka;
}
