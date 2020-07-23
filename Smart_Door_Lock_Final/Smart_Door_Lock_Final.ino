#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

 #define SS_PIN 10
 #define RST_PIN 9
 #define RELAY 3
 #define ACCESS_DELAY 3000
 
LiquidCrystal_I2C lcd (0x27, 16,2);
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup ()
{
  Serial.begin (9600);
  SPI.begin ();
  mfrc522.PCD_Init ();
  lcd.begin ();
  pinMode (RELAY, OUTPUT);

  lcd.setCursor (0,0);
  lcd.print ("Welcome To My House");
}

void loop() 
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Silahkan Tap");
  lcd.setCursor(0,1);
  lcd.print("Kartu Anda");

  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  content.toUpperCase();
  if (content.substring(1) == "04 27 22 12 89 56 80") //TUKAR UID INI SESUAI RFIA TAG KALIAN
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Akses Telah");
    lcd.setCursor(0,1);
    lcd.print("Diberikan");
    delay(1000);
    
    digitalWrite(RELAY, HIGH);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, LOW);
    
    
  }
    else {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Akses Ditolak");
    delay(1000);
  }
}
