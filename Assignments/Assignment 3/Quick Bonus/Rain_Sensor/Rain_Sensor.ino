#include<ESP32Servo.h>


#define led 23
#define rain_sensor 35
#define servo_pin 5

Servo servo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  servo.attach(servo_pin);
  servo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(rain_sensor);
  Serial.print("Value -> ");
  Serial.println(value);

  if(value < 1500){
    digitalWrite(led , HIGH);
    Serial.println("Heavy rain led on");
    servo.write(180);
    delay(50);

  }
  else {
    digitalWrite(led,LOW);
    servo.write(0);
    delay(50);
  }
}
