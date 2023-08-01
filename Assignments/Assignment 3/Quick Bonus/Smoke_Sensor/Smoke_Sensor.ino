#define led 11 // led pin
#define buzzer 10 // buzzer pin
#define smoke A0  // smoke sensor pin (analog)

#define sensor_threshold 300 // threshold value 
int sensor_reading ;

void setup() {
Serial.begin(9600); 
pinMode(buzzer,OUTPUT);
pinMode(led,OUTPUT);
pinMode(smoke,INPUT);
}


void loop() {
sensor_reading = analogRead(smoke); // getting sensor reading from A0 pin
Serial.print("Smoke sensor reading is :  "); // printing sensor inputs
Serial.println(sensor_reading);

// cheking if the reading reached to the threshold or not
if(sensor_reading>=sensor_threshold)
{
  tone(buzzer,1000,200);
  digitalWrite(led,HIGH);
}
else
{
noTone(buzzer);
digitalWrite(led,LOW);
}
delay(250);
}
