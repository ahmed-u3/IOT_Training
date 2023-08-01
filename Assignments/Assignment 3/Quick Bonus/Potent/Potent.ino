#include <ESP32Servo.h>

#define poten 25
#define servo_pin 5
#define step_delay 50
Servo servo;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(poten,INPUT);
  servo.attach(servo_pin);
  servo.write(0);
  delay(step_delay);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int signal = analogRead(poten);
  Serial.print("poten value -> ");
  Serial.println(signal);
  signal = map(signal ,  0 , 4095  , 0 , 180);
  Serial.print("Angle -> ");
  Serial.println(signal);
  servo.write(signal);
  delay(step_delay);
}