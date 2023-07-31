#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int sensorPin = A0; // select the input pin for LDR
int ledPin = 12;
int sensorValue = 0; // variable to store the value coming from the sensor
#define IRSensor A1
#define potpin A3
int potval =  0;

void setup() 
{
Serial.begin(9600); //sets serial port for communication
pinMode(12,OUTPUT);
pinMode(2, INPUT_PULLUP);
}

void loop() 
{
  sensorValue = analogRead(sensorPin); // read the value from the sensor
  Serial.println(sensorValue);
  myservo.attach(9);
  int buttonval = digitalRead(2);
  
  int IRValue = digitalRead(IRSensor);
  if( IRValue == 0 && buttonval == HIGH)
    {
      myservo.write(175);
     // myservo.write(potval);
     
    digitalWrite(ledPin,HIGH);  
    
    }
  
  else 
  {
    myservo.write(0);
    digitalWrite(ledPin,LOW);  
      
  if(sensorValue>=850)
  {
  
    myservo.write(175);

  }
  else
  {
    myservo.write(0);
    
    potval = analogRead(potpin);
    potval = map(potval,0,1023,0,175);
    myservo.write(potval);
  }
  }
  delay(500);
}
