/*
 * get_the_worm.ino
 * main arduino file for project get_the_worm
 * it's just a simple alarm clock
 * note the clock is in military time because I don't want an AM/PM flag
 * 
 * Brian J Gravelle (bgravelle5@gmail.com)
 *
 * First started: Sept. 23 2015
 * Last modified: Sept. 23 2015
*/

#include <TimerOne.h>

#define LED_PIN 2
#define START_SEC 30
#define START_MIN 0
#define START_HOU 0
#define ALARM_MIN 1
#define ALARM_HOU 0

char second, minute, hour;
bool is_alarm; 
 
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

void setup() {

  pinMode(LED_PIN, OUTPUT);    
  
  Timer1.initialize(1000000); // Period = 1 sec / Freq = 10Hz
  Timer1.attachInterrupt( timer_isr );
  
  second = START_SEC;
  minute = START_MIN;
  hour   = START_HOU;
  
}
 
void loop() {
  
}
 

void timer_isr() {
   increment_time();
   check_alarm();
