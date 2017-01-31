#include <EnableInterrupt.h>

#define touch_pin 4
#define ir_pin 2
#define m1 5
#define m2 6

volatile int dir=0; //

void ir_fn()
{
dir=0; 
//Serial.print("Dir (IR): ");
//Serial.println(dir);
disableInterrupt(ir_pin);
}

void touch_fn()
{
//disableInterrupt(ir_pin);
dir=1;
delay(150);
//Serial.print("Dir (TOUCH): ");
//Serial.println(dir); 
enableInterrupt(ir_pin, ir_fn, FALLING);
}
void setup() {
  // put your setup code here, to run once:
pinMode(m1,OUTPUT);
pinMode(m2,OUTPUT);
pinMode(touch_pin, INPUT_PULLUP);
pinMode(ir_pin, INPUT_PULLUP);
//enableInterrupt(ir_pin, ir_fn, FALLING);
//enableInterrupt(touch_pin, touch_fn, FALLING);
//Serial.begin(115200);
}




void loop() {
  // put your main code here, to run repeatedly:
bool touch=digitalRead(touch_pin);
bool ir=digitalRead(ir_pin);
//Serial.print("Touch: ");
//Serial.print(touch);
///Serial.print("IR: ");
//Serial.println(ir);

if (!ir)
dir=0;
if (!touch)
dir=1;

//Serial.print("Dir: ");
//Serial.println(dir);

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
delay(250);
//disableInterrupt(ir_pin); 
}
