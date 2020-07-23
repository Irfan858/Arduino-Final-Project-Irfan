#include <Wire.h>
#include <LiquidCrystal_I2C.h> 
#include <IRremote.h>


// Pin Interface
int relay1 = 7;
int relay2 = 6;
int relay3 = 8;
int relay4 = 9;
int RECV_PIN = 10;
const int LDR_Pin = A0;

//LCD Interface
LiquidCrystal_I2C lcd(0x27, 16,2);

//Variabel Data Setup
char data = 0;
int sensorValue = 0;
int ledHidup[] = {0,0,0,0};

//IR Interface
IRrecv irrecv(RECV_PIN);
decode_results results;


//IR Remote Button Setup
#define switch1 0xFF30CF
#define switch2 0xFF18E7
#define switch3 0xFF7A85

void setup() 
  {
    Serial.begin(9600);
     
     irrecv.enableIRIn();
     irrecv.blink13(true);
   
    //Pin Setup
    pinMode(relay1,OUTPUT);
    pinMode(relay2,OUTPUT);
    pinMode(relay3,OUTPUT);
    pinMode(relay4,OUTPUT);

    //LCD Setup
    lcd.begin();
  }



void loop()
{
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Smart Home");
    lcd.setCursor(0,1);
    lcd.print("By START-U TECH");
    
/*==========Fungsi IR========*/ 
    if (irrecv.decode(&results)) 
      {
        lcd.clear();
        unsigned int value = results.value;
        switch(value) 
          {
            case switch1:
            if(ledHidup[1] == 1) 
              {
                digitalWrite(relay1,LOW);
                lcd.setCursor(0,0);
                lcd.print("Lampu Ruang Tamu");
                lcd.setCursor(5,1);
                lcd.print("MATI");
                ledHidup[1] = 0;
              }
            else 
              {
                digitalWrite(relay1,HIGH);
                lcd.setCursor(0,0);
                lcd.print("Lampu R. Tamu");
                lcd.setCursor(5,1);
                lcd.print("HIDUP");
                ledHidup[1] = 1;
              }
            break;
      
            case switch2:
            if(ledHidup[2] == 1) 
              {
                digitalWrite(relay2,LOW);
                lcd.setCursor(0,0);
                lcd.print("Lampu R. Kamar");
                lcd.setCursor(5,1);
                lcd.print("MATI");
                ledHidup[2] = 0;
              }
            else
              {
                digitalWrite(relay2,HIGH);
                lcd.setCursor(0,0);
                lcd.print("Lampu R. Kamar");
                lcd.setCursor(5,1);
                lcd.print("HIDUP");
                ledHidup[2] = 1;
              }
            break;
            
            case switch3:
            if (ledHidup[3] == 1) 
              {
                digitalWrite(relay3,LOW);
                lcd.setCursor(0,0);
                lcd.print("Lampu R. Dapur");
                lcd.setCursor(5,1);
                lcd.print("HIDUP");
                ledHidup[3] = 0;
              }
            else
              {
                digitalWrite(relay3,HIGH);
                lcd.setCursor(0,0);
                lcd.print("Lampu R. Dapur");
                lcd.setCursor(5,1);
                lcd.print("MATI");
                ledHidup[3] = 1;
              }
            break;
        }
      Serial.println(value);
      irrecv.resume();
    }

/*=============  Cek bluetooth dan aksi============*/
   
   if(Serial.available() > 0) 
    {
      data = Serial.read();
      Serial.print(data);
      Serial.print("\n");
      
      //Kontrol Relay
      if (data == '0')
        {
          digitalWrite(relay1,LOW);
          lcd.setCursor(0,0);
          lcd.print("Lampu R. Tamu");
          lcd.setCursor(5,1);
          lcd.print("MATI");
        }
      else if (data == '1')
        {
          digitalWrite(relay1,HIGH);
          lcd.setCursor(0,0);
          lcd.print("Lampu R. Tamu");
          lcd.setCursor(5,1);
          lcd.print("HIDUP");
        }
      else if (data == '2')
        {
          digitalWrite(relay2,LOW);
          lcd.setCursor(0,0);
          lcd.print("Lampu R. Kamar");
          lcd.setCursor(5,1);
          lcd.print("MATI");
        } 
      else if (data == '3')
        {
          digitalWrite(relay2,HIGH);
          lcd.setCursor(0,0);
          lcd.print("Lampu R. Kamar");
          lcd.setCursor(5,1);
          lcd.print("HIDUP");
        } 
      else if (data == '4')
        {
          digitalWrite(relay3,LOW);
          lcd.setCursor(0,0);
          lcd.print("Lampu R. Dapur");
          lcd.setCursor(5,1);
          lcd.print("MATI");
        } 
      else if (data == '5')
        {
          digitalWrite(relay3,HIGH);
          lcd.setCursor(0,0);
          lcd.print("Lampu R. Dapur");
          lcd.setCursor(5,1);
          lcd.print("HIDUP");
        }
        
    }

/*========== Lampu LDR   =================*/
 
  //Lampu LDR
  sensorValue = analogRead(LDR_Pin);
  if (sensorValue > 600) 
    {
      digitalWrite (relay4, HIGH);
    } 
  else 
    {
      digitalWrite (relay4,LOW); 
    }
  delay(1000);
}
