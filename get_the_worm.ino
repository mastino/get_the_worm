/*
 * get_the_worm.ino
 * main arduino file for project get_the_worm
 * it's just a simple alarm clock
 * note the clock is in military time because I don't want an AM/PM flag
 * 
 * Brian J Gravelle (bgravelle5@gmail.com)
 *
 * First started: Sept. 23 2015
 * Last modified: Dec. 12 2015
*/

#include <TimerOne.h>
#include <LiquidCrystal.h>

#define LED_PIN 2
#define BUT_PIN 7
#define BUZ_PIN 4

#define START_SEC 00
#define START_MIN 10
#define START_HOU 9
#define ALARM_MIN 0
#define ALARM_HOU 7
#define PUSH_TIME 30000 //5 minutes in milliseconds
//#define PUSH_TIME 300000 //5 minutes in milliseconds
//#define PUSH_TIME 180000 //3 minutes in milliseconds
//#define PUSH_TIME 60000 //1 minute in milliseconds
#define PUSH_MULT 1

#define THIRTY_S  30000
#define ONE_MIN   60000

char second, minute, hour;
bool is_alarm; 
bool is_half_count = false;

LiquidCrystal lcd(3, 5, 8, 9, 10, 11);

void check_alarm();
void ring_alarm_hold();
void ring_alarm_pushup();
void do_excercise(char* message, unsigned long ex_time);
void wait_for_button();
void increment_time();
void print_time();

void setup() {

  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(BUZ_PIN, LOW);
  pinMode(BUT_PIN, INPUT); 
  pinMode(BUZ_PIN, OUTPUT);
  digitalWrite(BUZ_PIN, LOW);   
  
  Timer1.initialize(499900); // Period = 0.5 sec / Freq = 20Hz
  Timer1.attachInterrupt( timer_isr );
  
  second = START_SEC;
  minute = START_MIN;
  hour   = START_HOU;
  
  lcd.begin(16, 2);
  print_time();
  print_alarm();
  
}
 
void loop() {
   if(is_alarm) {
      ring_alarm_pushup();
   }
}
 

void timer_isr() {
   is_half_count = !is_half_count;
   if(!is_half_count) {
      increment_time();
      check_alarm();
   }
}

void ring_alarm_pushup(){
  
   wait_for_button(); 
   do_excercise("Pushups GO!", ONE_MIN);
   wait_for_button();   
   do_excercise("Forward Plank", THIRTY_S);
   wait_for_button();   
   do_excercise("L Side Plank", THIRTY_S);
   wait_for_button();   
   do_excercise("R Side Plank", THIRTY_S);
   wait_for_button(); 
   do_excercise("Pushups GO!", ONE_MIN);
   wait_for_button();   
   do_excercise("Forward Plank", THIRTY_S);
   wait_for_button();   
   do_excercise("L Side Plank", THIRTY_S);
   wait_for_button();   
   do_excercise("R Side Plank", THIRTY_S);
   wait_for_button(); 
   do_excercise("Pushups GO!", ONE_MIN);
   wait_for_button();    
  
   is_alarm = false;
   lcd.clear();
   print_time();
   print_alarm();
   digitalWrite(LED_PIN, LOW);

}

void wait_for_button() {
   digitalWrite(LED_PIN, HIGH);
   digitalWrite(BUZ_PIN, HIGH);
   while(digitalRead(BUT_PIN) != HIGH);
   digitalWrite(BUZ_PIN, LOW);
}

void do_excercise(char* message, unsigned long ex_time) {
   unsigned long time;
  
   time = millis();
   lcd.clear();
   lcd.setCursor(0, 1);
   lcd.print(message);
   while( (millis() - time) < ex_time ) {
     delay(100);
     digitalWrite(LED_PIN, HIGH);
     delay(100);
     digitalWrite(LED_PIN, LOW);
   }
}

void ring_alarm_hold(){
   unsigned long time;
   
   digitalWrite(LED_PIN, HIGH);
   digitalWrite(BUZ_PIN, HIGH);
   while(digitalRead(BUT_PIN) != HIGH);
   digitalWrite(BUZ_PIN, LOW);

   for (int i = 0; i < PUSH_MULT; i++) {
     time = millis();
     do {
       digitalWrite(BUZ_PIN, LOW);
       digitalWrite(LED_PIN, HIGH);
       
       while(digitalRead(BUT_PIN) != HIGH) {
          digitalWrite(BUZ_PIN, HIGH);
          time = millis();
       }
       
       delay(100);
       digitalWrite(LED_PIN, LOW);
       delay(100);
     } while( (millis() - time) < PUSH_TIME );
   }
   digitalWrite(LED_PIN, HIGH);
   is_alarm = false;
   digitalWrite(LED_PIN, LOW);
}

//only starts alarm
void check_alarm() { 
  if( (hour == ALARM_HOU) && (minute == ALARM_MIN) )
     is_alarm = true;
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
   print_time();
}


void print_alarm() {
  lcd.setCursor(0, 1);
  lcd.print("Alarm: ");
  lcd.print(ALARM_HOU/10);
  lcd.print(ALARM_HOU%10);
  lcd.print(":");
  lcd.print(ALARM_MIN/10);
  lcd.print(ALARM_MIN%10);
  lcd.print(":00");
}

void print_time() {
  lcd.setCursor(0, 0);
  lcd.print("Time:  ");
  lcd.print((int)(hour/10));
  lcd.print((int)(hour%10));
  lcd.print(":");
  lcd.print((int)(minute/10));
  lcd.print((int)(minute%10));
  lcd.print(":");
  lcd.print((int)(second/10));
  lcd.print((int)(second%10));
}
