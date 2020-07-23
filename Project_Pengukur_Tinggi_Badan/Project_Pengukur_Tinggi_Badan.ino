#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define echo 9
#define trigger 8

int durasi;
int panjang;
int total;


void setup() {
  // put your setup code here, to run once:
pinMode (trigger,OUTPUT);
pinMode (echo,INPUT);
Serial.begin (9600);
 lcd.begin(); 
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(trigger, LOW);
delayMicroseconds(2);
digitalWrite(trigger, HIGH);
delayMicroseconds(10);
digitalWrite(trigger, LOW);

durasi = pulseIn (echo, HIGH);

panjang = (durasi*0.034/2);
total = (202-panjang);

lcd.setCursor(0,0);
  lcd.print("-=Tinggi Badan=-");
lcd.setCursor(6,1);
  lcd.print(total);

Serial.print("panjang = ");Serial.print(panjang); Serial.print ("      tinggi = ");
  Serial.println (total); 

  delay(2000);
  lcd.clear();
}
