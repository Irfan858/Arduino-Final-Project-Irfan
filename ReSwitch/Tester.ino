#include "Countimer.h"
#define Relay 7
long set,jam, menit,detik;


Countimer timer;

void setup() {
  Serial.begin(9600);

    // Set up count down timer with 10s and call method onComplete() when timer is complete.
    // 00h:00m:10s
  
    // Print current time every 1s on serial port by calling method refreshClock().
    timer.setInterval(refreshClock, 1000);
    pinMode(13,OUTPUT);
    pinMode(Relay,OUTPUT);

}

void refreshClock() 
{
    Serial.print("Current time is: ");
    Serial.println(timer.getCurrentTime());
    digitalWrite(13,HIGH);
    digitalWrite(Relay,HIGH);
}

void(*restart)(void)=0;

void onComplete() 
{
  Serial.println("Complete Waktu Habis Gan!!!");
  delay (500);
  digitalWrite(13,LOW);
  digitalWrite(Relay,LOW);
  restart();  
}

void loop() {
  // Run timer

  if (Serial.available() > 0) 
  {
    set = Serial.parseInt();
    // read the incoming byte:
    if ( set == 99)
    {
      jam = Serial.parseInt();
      menit = Serial.parseInt();
      detik = Serial.parseInt();
      // say what you got:
      Serial.print("Waktu di Set : ");
      Serial.print(jam);
      Serial.print("  ");
      Serial.print(menit);
      Serial.print("  ");
      Serial.println(detik);

      timer.setCounter(jam,menit,detik, timer.COUNT_DOWN, onComplete);
    }
  }

  
if ( detik !=0 or menit != 0 or jam != 0)
{
  
    timer.run();

    // Now timer is running and listening for actions.
    // If you want to start the timer, you have to call start() method.
    if(!timer.isCounterCompleted()) 
      {
       timer.start();
      }
}
}
