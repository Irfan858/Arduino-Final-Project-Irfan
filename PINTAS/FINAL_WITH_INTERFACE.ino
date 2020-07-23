//Inisialisasi library
#include <Wire.h>
#include <DS3231.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Button.h>
#include <EEPROM.h>

//Inisialisasi Tombol
int pin_bwh  = 3;
int pin_ats  = 5;
int pin_menu = 2;
int pin_trs  = 4;

//Inisialisasi Relay dan buzzer
int relay   = 8;
int buzzer  = 9;

//fungsi Pullup dan Invert
bool PULLUP = true;
bool INVERT = true;
#define DEBOUNCE_MS 20
#define REPEAT_FIRST 500
#define REPEAT_INCR 100

//Deklarasi Fungsi Tombol
Button bawah  (pin_bwh, PULLUP, INVERT, DEBOUNCE_MS);
Button atas   (pin_ats, PULLUP, INVERT, DEBOUNCE_MS);
Button menu   (pin_menu, PULLUP, INVERT, DEBOUNCE_MS);
Button terus  (pin_trs, PULLUP,  INVERT, DEBOUNCE_MS);

//Enumerasi dan Definisi Fungsi
enum {WAIT, INCR, DECR};
uint8_t STATE;
int count;
int lastCount = -1;
unsigned long rpt = REPEAT_FIRST;

//Inisialisasi OLED
Adafruit_SSD1306 display(-1);

//Inisialisasi EEPROM
int addr_AlarmJam = 1;
int addr_AlarmMenit = 2;
uint8_t value;
//uint8_t AlarmJam;
//uint8_t AlarmMenit;

//Inisialisasi Dan Fungsi RTC
DS3231 rtc(SDA,SCL);
Time t;
uint32_t targetTime = 0;
uint8_t conv2d(const char*p)
{
  uint8_t v =0;
  if ('0' <= *p && *p <= '9')
  v = *p - '0';
  return 10 * v + *++p - '0';
}

  
//Fungsi Waktu
uint8_t jam = 0, menit = 0, detik = 0, tanggal = 0, bulan = 0;
int tahun = 0;
String hari = " ";
uint8_t alarmJam = EEPROM.read(addr_AlarmJam);
uint8_t alarmMenit = EEPROM.read(addr_AlarmMenit);
int alarmLama = 2;
uint8_t setMode = 0, setAlarm = 0, alarmMode = 0;

//Konfigurasi Header
char header[]="Penyiram Tanaman Otomatis";
int x, minX; 

void setup()
{
  //Inisialisasi Serial
  Serial.begin(9600);

  //inisallisasi Fungsi Relay dan Buzzer
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);

   //Inislalisasi RTC
   rtc.begin();

  // Inisialisasi Fungsi Relay
  pinMode(relay,OUTPUT);

  
  
   //Inisialisasi OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  //Splash Screen
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(15,0);
  display.print("Penyiram Tanaman");

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(40,10);
  display.print("Otomatis");
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(35,40);
  display.print("Powered By");
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25,50);
  display.print("Start-U Tech");

 //Konfigurasi Header
  x     = display.width();
  minX  = -10*strlen(header);
  
  display.display();
  delay(3000);

  

   
}
void loop() {
 t = rtc.getTime();
 hari = rtc.getDOWStr(1);

 if (setMode == 0)
 {
  jam     = t.hour,DEC;
  menit   = t.min,DEC;
  detik   = t.sec,DEC;
  tanggal = t.date,DEC;
  bulan   = t.mon,DEC;
  tahun   = t.year,DEC;  
 }
  if (setAlarm <2)
  {
  display.clearDisplay();
     //Header
  display.setTextSize(1);
  display.setCursor(x,0);
  display.setTextWrap(false);
  display.print(header);
  x= x-6;
  if (x < minX) x = display.width();
  
   display.setTextSize(1);
   display.setCursor(25,50);
   if(setMode==0)
   display.print(hari);
   else
   {
    display.print("Set");
   }
   display.print(" ");
   display.print(tanggal);
   display.print("-");
   display.print(bulan);
   display.print("-");
   display.print(tahun);
   display.print("  ");

   display.setCursor(15,20);
   display.setTextSize(2);
   if (jam<10)
   {
    display.print(F("0"));
   }
   display.print(jam);
   display.print(":");
   if (menit<10)
   {
   display.print(F("0"));
   }
   display.print(menit);
   display.print(F(":"));
   if (detik<10)
   {
    display.print(F("0"));
   }
   display.print(detik);
   
  }

  setupClock();
  Alarm (alarmJam, alarmMenit, alarmLama);

 if (setAlarm <2 && setMode !=0)
  {
    delay(100);
  }
 if (setAlarm <2 && setMode == 1)
  {
    display.setTextSize(1);
    display.setCursor (20,10);
    display.print (F("  "));
    delay(100);
  }
 if (setAlarm <2 && setMode == 2 && bulan>9)
  {
    display.setTextSize(1);
    display.setCursor(40,10);
    display.print(F(" "));
    delay(100);
  }
 if (setAlarm <2 && setMode == 2 && bulan<=9)
  {
    display.setTextSize(1);
    display.setCursor(40,10);
    display.print(F(" "));
    delay(100);
  }
 if (setAlarm <2 && setMode == 3 && bulan >9)
 {
  display.setTextSize(1);
  display.setCursor(60,10);
  display.print(F(" "));
  delay(100);
 }
  if (setAlarm <2 && setMode == 3 && bulan <= 9)
 {
  display.setTextSize(1);
  display.setCursor(50,10);
  display.print(F(" "));
  delay(100);
 }
 if (setAlarm <2 && setMode == 4)
 {
  display.setTextSize(1);
  display.setCursor(10,30);
  display.print(F(" "));
  delay(100);
 }
 if (setAlarm <2 && setMode == 5)
 {
  display.setTextSize(1);
  display.setCursor(30,30);
  display.print(F(" "));
  delay(100);
 }
 if (setAlarm <2 && setMode == 6)
 {
  display.setTextSize(1);
  display.setCursor(50,30);
  display.print(F(" "));
  delay(100);
 }

 if (setMode == 0 && setAlarm != 0 && setAlarm != 1)
 {
  delay(100);
 }

 if (setMode == 0 && setAlarm == 2)
 {
  display.setTextSize(1);
  display.setCursor(35,20);
  display.print(F(" "));
  delay(100);
  }
 if (setMode == 0 && setAlarm ==3)
  {
    display.setTextSize(1);
    display.setCursor(70,20);
    display.print(F(" "));
    delay(100);
  }
 
 if (setMode == 0 && setAlarm == 4 && alarmLama >9)
{
  display.setTextSize(1);
  display.setCursor(40,50);
  display.print(F("  "));
  delay(100);
  }
if (setMode == 0 && setAlarm == 4 && alarmLama <10)
  {
  display.setTextSize(1);
  display.setCursor(40,50);
  display.print(F("  "));
  delay(100);
  } 

  Serial.print (setMode);
  Serial.print (" ");
  Serial.print (setAlarm);
  display.display();
 }

 void setupClock (void)
 {
  //Baca Tombol
  bawah.read();
  atas.read();
  menu.read();
  terus.read();

  if (setMode == 7)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10,10);
    display.print(F("Waktu Telah Di Atur"));
    delay (1000);
    rtc.setTime (jam, menit, detik);
    rtc.setDate (tanggal, bulan, tahun);
    display.display();
    display.clearDisplay();
    setMode = 0;
  }

  if (setAlarm == 5)
  {
    display.clearDisplay();
    display.setCursor(10,10);
    display.print (F("Alarm Selesai Atur"));
    display.setCursor(10,30);
    display.print("Pada :  ");
    display.print(alarmJam);
    display.print(":");
    display.print(alarmMenit);
    display.display();
    delay (1000);
    //Simpan Jam Penyiraman ke EEPROM
    value = alarmJam;
    EEPROM.write(1, value);

    //Simpan Menit Ke EEPROMvalue = alarmMenit;
    value = alarmMenit;
    EEPROM.write(2, value); 
    setAlarm=0;
    alarmMode=1;  
    
  }

  if (setAlarm >0)
  {
    alarmMode=0;
  }

  switch  (STATE)
  {
    case WAIT:
      if (menu.wasPressed())
        {
            setMode = setMode+1;
        }
      if (terus.wasPressed())
        {
          setAlarm = setAlarm+1;  
        }
      if (atas.wasPressed())
        STATE = INCR;
      else if (bawah.wasPressed())
        STATE = DECR;
      else if (atas.wasReleased())
        rpt = REPEAT_FIRST;
      else if (bawah.wasReleased())
        rpt = REPEAT_FIRST;
      else if (atas.pressedFor(rpt))
      {
        rpt += REPEAT_INCR;
        STATE = INCR;
      }
      else if (bawah.pressedFor(rpt))
      {
        rpt += REPEAT_INCR;
        STATE = DECR;
      }

      break;

      case INCR:
        if (setAlarm < 2 && setMode == 1 && tanggal < 31)
          tanggal = tanggal+1;
        if (setAlarm < 2 && setMode == 2 && bulan < 12)
          bulan = bulan+1;
        if (setAlarm < 2 && setMode == 3 && tahun < 2099)
          tahun = tahun+1;
        if (setAlarm < 2 && setMode == 4 && jam < 23)
          jam = jam+1;
        if (setAlarm < 2 && setMode == 5 && menit < 60)
          menit = menit+1;
        if (setAlarm < 2 && setMode == 6 && detik < 60)
          detik = detik+1;
        if (setMode == 0 && setAlarm == 2 && alarmJam < 23)
          alarmJam = alarmJam+1;
        if (setMode == 0 && setAlarm == 3 && alarmMenit < 59)
          alarmMenit = alarmMenit+1;
        if (setMode == 0 && setAlarm == 4 && alarmLama < 59)
          alarmLama = alarmLama+1;
        STATE = WAIT;
        break;

        
      case DECR:
        if (setAlarm < 2 && setMode == 1 && tanggal > 0)
          tanggal = tanggal-1;
        if (setAlarm < 2 && setMode == 2 && bulan > 0)
          bulan = bulan-1;
        if (setAlarm < 2 && setMode == 3 && tahun > 2000)
          tahun = tahun-1;
        if (setAlarm < 2 && setMode == 4 && jam > 0)
          jam = jam-1;
        if (setAlarm < 2 && setMode == 5 && menit > 0)
          menit = menit-1;
        if (setAlarm < 2 && setMode == 6 && detik > 0)
          detik = detik-1;
        if (setMode == 0 && setAlarm == 2 && alarmJam > 0)
          alarmJam = alarmJam-1;
        if (setMode == 0 && setAlarm == 3 && alarmMenit > 0)
          alarmMenit = alarmMenit-1;
        if (setMode == 0 && setAlarm == 4 && alarmLama > 0)
          alarmLama = alarmLama-1;
        STATE = WAIT;
        break;
  }
}

void Alarm (uint8_t alarmJam, uint8_t alarmMenit, int alarmLama)
{
   if (alarmMode==1 && jam == alarmJam && (menit - alarmMenit >= 0 ) && (menit - alarmMenit <= alarmLama ))
  {
    tone(13, 1200);
    pinMode(relay,LOW);
    terus.read();
    if (terus.wasPressed())
    {
      alarmMode = 0;
    }
  }
  else
  {
    noTone (13);
    pinMode(relay,HIGH);
  }

   if (setMode == 0 && setAlarm !=0 && setAlarm !=1)
 {
  display.clearDisplay();
  display.setCursor (0,0);
  display.print(F("Set Waktu Penyiraman"));
  
  display.setCursor (35,20);
  display.setTextSize (2);
  if(alarmJam<10)
  {
    display.print(F("0"));
    }
  display.print (alarmJam);
  display.print(F(":"));
  if(alarmMenit<10)
  {
    display.print(F("0"));
    }
  display.print (alarmMenit);

  display.setCursor(0,50);
  display.setTextSize(1);
  display.print(F("  Long:"));
  display.print (alarmLama);
  display.print(F("mnt "));
 }
}
