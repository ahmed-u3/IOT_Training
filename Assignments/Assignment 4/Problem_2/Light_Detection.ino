#include <ESP32Servo.h>
#include <LiquidCrystal.h>

#define ldr_pin 34
#define led_pin 5
#define ir_pin 35
#define button 2
#define servo_pin 27

LiquidCrystal lcd(19, 14, 26, 32, 33, 25); // RS, E, D4, D5, D6, D7
Servo myservo;

bool button_signal;
int ldr_signal;
int ir_signal;
int servo_signal;

// these 4 variables are used so the other sensors can operate while search is activated because the when search is applied without them it blocks the other functions
unsigned long previousMillis = 0;
const unsigned long interval = 10;
int angle = 0;
int direction = 1; // 1 for moving from 0 to 180, -1 for moving from 180 to 0

void search() {
  unsigned long currentMillis = millis();               // gets the current time
  if (currentMillis - previousMillis >= interval) {     // check if enough time has passed since the last update to the servo
    previousMillis = currentMillis;                     // set the last time to the current time

    myservo.write(angle);
    angle += direction * 2; // Increment or decrement the angle for smooth servo movement

    lcd.setCursor(0, 0);
    lcd.print("Servo Angle: ");
    lcd.setCursor(0, 1);
    lcd.print("             ");
    lcd.setCursor(0, 1);
    lcd.print(angle);
    delay(100);

    // here we keep the angle in a specified range 0 --> 180
    if (angle >= 180) {
      angle = 180; // Limit the angle to 180 degrees
      direction = -1; // Change direction to move from 180 to 0
    } else if (angle <= 0) {
      angle = 0; // Limit the angle to 0 degrees
      direction = 1; // Change direction to move from 0 to 180
    }
  }
}

void light_alarm(int signal){
  if(signal < 500){
      digitalWrite(led_pin, HIGH);
    }else{
      digitalWrite(led_pin, LOW);
    }
}

void setup() {
  // put your setup code here, to run once:

  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(ldr_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  pinMode(ir_pin, INPUT);

  myservo.attach(servo_pin);
}

void loop() {
  // put your main code here, to run repeatedly:

  ldr_signal = analogRead(ldr_pin);
  button_signal = digitalRead(button);
  ir_signal = analogRead(ir_pin);

  //Serial.println(button_signal);

  light_alarm(ldr_signal);

  if(button_signal == HIGH){
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("IR: ");
    lcd.print(ir_signal);
    lcd.print(" microns");

    lcd.setCursor(0, 1);
    lcd.print("LDR: ");
    lcd.print(ldr_signal);
    lcd.print(" ohm");

    Serial.println("***********************");
    Serial.println("IR-Reading: ");
    Serial.println(ir_signal);
    Serial.println("LDR-Reading: ");
    Serial.println(ldr_signal);
    Serial.println("***********************");
    delay(500);
    
    myservo.write(90);
  }else{
    if(ldr_signal < 500){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Warning!!");
      delay(150);
    }else{
      lcd.clear();
      search();
    }
  }
}