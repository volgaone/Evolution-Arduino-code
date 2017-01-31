#include "LedControlMS.h"
/*
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */

LedControl lc=LedControl(5,3,4,1);


#define trigPin 7
#define echoPin 8
#define m1 9
#define m2 10

int dir=0;
 
void setup() {
  // put your setup code here, to run once:
 lc.shutdown(0,false);
  lc.setIntensity(0,3);
  lc.clearDisplay(0);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);

  Serial.begin(115200);

}



void DrawLCD(char level)
{
  // put your main code here, to run repeatedly:
  char row=0;
  //lc.clearDisplay(0);
  if (level>8) level=8;
  while (row<level)
 lc.setRow(0,row++,0xFF);
 while (row<9)
 lc.setRow(0,row++,0x00);

}
void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH,200000);
  distance = (duration/2) / 29.1;
 
DrawLCD((char)(distance-15)/6);


if (distance<20)
dir=1;
else if (distance>60)
dir=0;


if (dir==1)
{
digitalWrite(m1,LOW);
digitalWrite(m2,HIGH);  
}
else if (dir==0)
{
digitalWrite(m2,LOW);  
digitalWrite(m1,HIGH);
}

Serial.print("Dir: ");
Serial.println(dir);


delay(100);

  
}
