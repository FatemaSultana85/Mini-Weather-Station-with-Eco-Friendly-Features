#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>
#include "RTClib.h"

Servo myservo1, myservo2;

LiquidCrystal lcd1(2,3,4,5,6,7);
LiquidCrystal lcd2(48,49,50,51,52,53);
LiquidCrystal lcd3(41,42,43,44,45,46);

RTC_DS1307 RTC;

int moisture_value;
int moist_percent;
int motor_forward1 = 22;
int motor_reverse1 = 23;

int motor_forward2 = 24;
int motor_reverse2 = 25;

#define echo 14
#define trigger 15
//#define tank_pump 4

int led_red = 26;
int led_yellow = 27;
int led_green = 28;
long duration;
int distance;
int distance_percent;
int pos = 0;


int value = 0;
int lm35 = A15;

const int buzzering1 = 38;
const int buzzering2 = 39;

void setup()
{

  //Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(motor_forward1, OUTPUT);
  pinMode(motor_reverse1, OUTPUT);

  pinMode(motor_forward2, OUTPUT);
  pinMode(motor_reverse2, OUTPUT);

  pinMode(echo,INPUT);
  pinMode(trigger,OUTPUT);
  digitalWrite(trigger,LOW);
  //pinMode(tank_pump,OUTPUT);
  //digitalWrite(tank_pump,LOW);
  pinMode(led_red, OUTPUT);
  pinMode(led_yellow, OUTPUT);
  pinMode(led_green, OUTPUT);

  lcd1.begin(20,4);
  lcd1.setCursor(0,0);
  lcd1.print(" Weather Station ");
  delay(500);
  lcd1.clear(); 

  lcd2.begin(20, 4);
  lcd2.setCursor(0,1);
  lcd2.print("Automatic");
  lcd2.setCursor(0,2);
  lcd2.print("Watering");
  lcd2.setCursor(0,3);
  lcd2.print("System");
  delay(500);
  lcd2.clear(); 

  Wire.begin();
  RTC.begin();
  lcd3.begin(20, 4);

  if (! RTC.isrunning()) 
  {
    Serial.println("RTC is NOT running!");    
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }

  pinMode(buzzering1, OUTPUT);
  pinMode(buzzering2, OUTPUT);

  myservo1.attach(29);
  myservo2.attach(30);
  myservo1.write(0);
  myservo2.write(0);

}

void loop()
{

  dateandtime();

  measuretemp();

  moisture_value= analogRead(A0);
  moist_percent=map(moisture_value,0,1023,0,100);
  lcd1.setCursor(0,1);
  lcd1.print("MOIST CONTENT= ");
  lcd1.print(moist_percent);
  lcd1.print("%");

  soilmotor();

  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigger,LOW);
  duration=pulseIn(echo,HIGH);
  distance=duration*0.017; 
  distance_percent=map( distance,0,1023,0,100);
  condition();

  watertankmotor();

  myservo1.write(90);
  myservo2.write(90);

  buzzer1();
  buzzer2();

}

void buzzer1()
{
  if(moist_percent<50)
  {
    tone(buzzering1,1000);
    delay(10);
    noTone(buzzering1);
    delay(100);
  }
}

void buzzer2()
{
  if(distance_percent<10)
  {
    tone(buzzering2,1000);
    delay(10);
    noTone(buzzering2);
    delay(100);
  }
}

void soilmotor()
{
  if(moist_percent<75)
  {
    if(moist_percent<50)
    {
    digitalWrite(motor_forward1, HIGH);
    digitalWrite(motor_reverse1, LOW);
    lcd1.setCursor(0,3);
    lcd1.print("S-MOTOR STATUS ");
    lcd1.print("ON");
    delay(100);
    myservo1.write(0);
    myservo2.write(0);
   //delay(20);
    delay(1000);
    }
  }
  else 
  {

    digitalWrite(motor_forward1, LOW);
    digitalWrite(motor_reverse1, LOW);
    lcd1.setCursor(0,3);
    lcd1.print("S-MOTOR STATUS ");
    lcd1.print("OFF");
    delay(100);

  }

}

void watertankmotor()
{
  if(distance_percent<=99)
  {
    if(distance_percent<10)
    {
      digitalWrite(motor_forward2, HIGH);
      digitalWrite(motor_reverse2, LOW);
      lcd2.setCursor(0,2);
      lcd2.print("W-MOTOR STATUS ");
      lcd2.print("ON");
      delay(100);
    }
  }
  else
  {
    digitalWrite(motor_forward2, LOW);
    digitalWrite(motor_reverse2, LOW);
    lcd2.setCursor(0,2);
    lcd2.print("W-MOTOR STATUS ");
    lcd2.print("OFF");
    delay(100);
  }
}

void dateandtime()
{
  DateTime now = RTC.now();
lcd3.setCursor(0, 0);
lcd3.print(now.day(), DEC);
lcd3.print('/');
lcd3.print(now.month(), DEC);
lcd3.print('/');
lcd3.print(now.year(), DEC);
lcd3.print(' ');
lcd3.setCursor(0, 1);
switch(now.hour())
  {
    case 00:
    lcd3.print("12");
    lcd3.print(':');
    break;

    case 01:
    lcd3.print("01");
    lcd3.print(':');
    break;

    case 02:
    lcd3.print("02");
    lcd3.print(':');
    break;

    case 03:
    lcd3.print("03");
    lcd3.print(':');
    break;

    case 04:
    lcd3.print("04");
    lcd3.print(':');
    break;

    case 05:
    lcd3.print("05");
    lcd3.print(':');
    break;

    case 06:
    lcd3.print("06");
    lcd3.print(':');
    break;

    case 07:
    lcd3.print("07");
    lcd3.print(':');
    break;

    case 8:
    lcd3.print("08");
    lcd3.print(':');
    break;

    case 9:
    lcd3.print("09");
    lcd3.print(':');
    break;

    case 10:
    lcd3.print("10");
    lcd3.print(':');
    break;

    case 11:
    lcd3.print("11");
    lcd3.print(':');
    break;

    case 12:
    lcd3.print("12");
    lcd3.print(':');
    break;
    
    case 13:
    lcd3.print("01");
    lcd3.print(':');
    break;

    case 14:
    lcd3.print("02");
    lcd3.print(':');
    break;

    case 15:
    lcd3.print("03");
    lcd3.print(':');
    break;

    case 16:
    lcd3.print("04");
    lcd3.print(':');
    break;

    case 17:
    lcd3.print("05");
    lcd3.print(':');
    break;

    case 18:
    lcd3.print("06");
    lcd3.print(':');
    break;

    case 19:
    lcd3.print("07");
    lcd3.print(':');
    break;
    
    case 20:
    lcd3.print("08");
    lcd3.print(':');
    break;

    case 21:
    lcd3.print("09");
    lcd3.print(':');
    break;

    case 22:
    lcd3.print("10");
    lcd3.print(':');
    break;

    case 23:
    lcd3.print("11");
    lcd3.print(':');
    break;
  }
if (now.minute()<10)
lcd3.print('0');
lcd3.print(now.minute(), DEC);
lcd3.print(':');
if (now.second()<10)
lcd3.print('0');
lcd3.print(now.second(), DEC);
if(now.hour()>11)
{
  lcd3.print(" PM");
}
else
{
  lcd3.print(" AM");
}
delay(10);
}

void measuretemp()
{
  value = analogRead(lm35);
  float milivolts = (value/1024.0)*5000;
  float cel = milivolts/10;
  lcd3.setCursor(0, 2);
  lcd3.print("Today's Temperature: ");
  lcd3.setCursor(0, 3);
  lcd3.print(cel);
  lcd3.print(char(223));
  lcd3.print("C");
}

void condition(){
if (distance_percent<=90 && distance_percent>10){
LCD_1(); // Tank Pump OFF
digitalWrite(led_yellow,HIGH);
digitalWrite(led_red,LOW);
digitalWrite(led_green, LOW);
delay(100);
}
else if (distance_percent<=10)
{
LCD_2(); // Tank Pump ON
digitalWrite(led_red,HIGH);
digitalWrite(led_yellow,LOW);
digitalWrite(led_green, LOW);
}

//delay(100);
else if (distance_percent>=91)
{

LCD_3(); // Tank Pump OFF
digitalWrite(led_green,HIGH);
digitalWrite(led_red,LOW);
digitalWrite(led_yellow,LOW);
delay(100);

}

}

  void LCD_1()
  {
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("TANK LEVEL=  ");
  lcd2.print(distance_percent);
  lcd2.print("%");
  }

void LCD_2(){
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("TANK LEVEL=  ");
  lcd2.print(distance_percent);
  lcd2.print("%");
  }
  void LCD_3(){
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("TANK LEVEL=  ");
  lcd2.print(distance_percent);
  lcd2.print("%");
  }
