/*
 * get_the_worm.ino
 * main arduino file for project get_the_worm
 * it's just a simple alarm clock
 * note the clock is in military time because I don't want an AM/PM flag
 * 
 * Brian J Gravelle (bgravelle5@gmail.com)
 *
 * First started: Sept. 23 2015
 * Last modified: Sept. 26 2015
*/

#include <TimerOne.h>

#define LED_PIN 2
#define BUT_PIN 7
#define BUZ_PIN 4

#define START_SEC 0
#define START_MIN 25
#define START_HOU 23
#define ALARM_MIN 30
#define ALARM_HOU 6
//#define PUSH_TIME 300000 //5 minutes in milliseconds
#define PUSH_TIME 60000 //1 minute in milliseconds

char second, minute, hour;
bool is_alarm; 
bool is_half_count = false;

void increment_time();

//only starts alarm
void check_alarm();

void setup() {

  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(BUZ_PIN, LOW);
  pinMode(BUT_PIN, INPUT); 
  pinMode(BUZ_PIN, OUTPUT);
  digitalWrite(BUZ_PIN, LOW);   
  
  Timer1.initialize(500000); // Period = 0.5 sec / Freq = 20Hz
  Timer1.attachInterrupt( timer_isr );
  
  second = START_SEC;
  minute = START_MIN;
  hour   = START_HOU;
  
}
 
void loop() {

   unsigned long time;

   if(is_alarm) {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZ_PIN, HIGH);
      while(digitalRead(BUT_PIN) != HIGH);
      digitalWrite(BUZ_PIN, LOW);

      time = millis();
      do {
         while(digitalRead(BUT_PIN) != HIGH) {
           time = millis();
           digitalWrite(BUZ_PIN, HIGH);
           digitalWrite(LED_PIN, HIGH);
         }
         digitalWrite(BUZ_PIN, LOW);
         digitalWrite(LED_PIN, HIGH);
         delay(100);
         digitalWrite(LED_PIN, LOW);
         delay(100);

      } while( (millis() - time) < PUSH_TIME );
      is_alarm = false;
   }

}
 

void timer_isr() {
   is_half_count = !is_half_count;
   if(!is_half_count) {
      increment_time();
      check_alarm();
   }
}
   
void increment_time() {
   
   second++;
   if(second >= 60) {
      second = 0;
      minute++;
   }  
   if(minute >= 60) {
      minute = 0;
      hour++; 
   }
   if(hour >= 24) {
      hour = 0;
   }  

}

//only starts alarm
void check_alarm() { 
  
  if( (hour == ALARM_HOU) && (minute == ALARM_MIN) )
     is_alarm = true;
     
}
