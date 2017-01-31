#include <FrequencyTimer2.h>

#define M1_A 5
#define M1_B 6

#define M2_A 9
#define M2_B 10

#define S_LEFT 2
#define S_FRONT 3
#define S_RIGHT 4


int dir=1;

void setup() {
  // put your setup code here, to run once:
pinMode(M1_A, OUTPUT);
pinMode(M1_B, OUTPUT);
pinMode(M2_A, OUTPUT);
pinMode(M2_B, OUTPUT);
}

void go_forward()
{
  dir=1;
  digitalWrite(M1_A,HIGH);
 digitalWrite(M1_B,LOW); 

 digitalWrite(M2_A,HIGH);
 digitalWrite(M2_B,LOW); 
 delay(50);
}

void go_back()
{
 digitalWrite(M1_A,HIGH);
 digitalWrite(M1_B,LOW); 

 digitalWrite(M2_A,HIGH);
 digitalWrite(M2_B,LOW);
  
}

void turn_right_90()
{
 digitalWrite(M1_A,LOW);
 digitalWrite(M1_B,HIGH); 
}


void turn_left_90()
{
 digitalWrite(M1_A,LOW);
 digitalWrite(M1_B,HIGH);   
}


void Go_back_and_turn()
{
digitalWrite(M1_A,LOW);
 digitalWrite(M1_B,HIGH); 

 digitalWrite(M2_A,LOW);
 digitalWrite(M2_B,HIGH);
 delay(1750);
 
 digitalWrite(M1_A,LOW);
 digitalWrite(M1_B,LOW);

  delay (750);

  go_forward();
}


void loop() {
  // put your main code here, to run repeatedly:
if (digitalRead(S_FRONT)==0)
{
Go_back_and_turn();
//dir=0;
}
else dir=1;

if (dir==1)
{
 digitalWrite(M1_A,HIGH);
 digitalWrite(M1_B,LOW); 

 digitalWrite(M2_A,HIGH);
 digitalWrite(M2_B,LOW); 
 
  
  }
  if (dir==0)
  {
 digitalWrite(M1_A,LOW);
 digitalWrite(M1_B,HIGH); 

 digitalWrite(M2_A,LOW);
 digitalWrite(M2_B,HIGH);   
 }

delay(1);

}
