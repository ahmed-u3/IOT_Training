// Include necessary libraries
#include <LiquidCrystal.h>   // Library for the LCD display
#include <ESP32Servo.h>      // Library for controlling Servo motors
#include <Adafruit_AHTX0.h>  // Library for the AHT10/AHT20 temperature and humidity sensor
#include <Robojax_L298N_DC_motor.h>  // Library for controlling DC motors
#include "BluetoothSerial.h"  // Library for Bluetooth communication

// Initialize objects for AHT10/AHT20 sensor and Servo motor
Adafruit_AHTX0 aht;   // Temperature and humidity sensor object
Servo myservo;        // Servo motor object
BluetoothSerial SerialBT;  // Bluetooth serial communication object

sensors_event_t humidity, temp;  // Event objects for humidity and temperature

// Initialize the LCD module with the specified pin configuration
const int rs = 3, en = 23, d4 = 5, d5 = 18, d6 = 19, d7 = 16;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define the pins used for various components
const int trigPin = 33;      // Digital output pin to trigger the ultrasonic sensor
const int echoPin = 32;      // Digital input pin to receive the echo from the ultrasonic sensor

long duration;
int distance;

int buzzer = 17;             // Pin connected to the buzzer
int LED = 2;                // Pin connected to the LED

int flame_sensor = 4;      // Pin connected to the flame sensor
int flameReadings;         
int flame_detected;         // Variable to store the flame detection status

// Define the proximity threshold value (adjust as needed)
const int proximityThreshold = 10; // Threshold distance in centimeters

// Smoke sensor pin and threshold value
#define smoke 35
#define sensor_threshold 250
int smoke_reading;

// Motor 1 (right motor) settings
#define CHA 0
#define ENA 13 
#define IN1 12
#define IN2 14

// Motor 2 (left motor) settings
#define IN3 26
#define IN4 27
#define ENB 25 
#define CHB 1

const int CCW = 2;
const int CW  = 1;

#define motor1 1 // right motor
#define motor2 2 // left motor

// for two motors without debug information	
Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA,  IN3, IN4, ENB, CHB);

// for two motors with debug information
//Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA, IN3, IN4, ENB, CHB, true);

int servoPin = 15;
int pos = 0;    // variable to store the servo position

int count = 0;  // Counter 

String device_name = "ESP32-BT-Slave";  // Bluetooth device name

bool autonomousMode = true;  // Flag for autonomous mode

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Check if the AHT10/AHT20 sensor is connected and functioning
  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);  // Hangs the program in an infinite loop if sensor not found
  }
  Serial.println("AHT10 or AHT20 found");

  // Set the mode for LED and buzzer pins as OUTPUT
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Set the trigPin as an OUTPUT and the echoPin as an INPUT
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set the flame sensor pin as an INPUT
  pinMode(flame_sensor, INPUT);

  // Set up the number of columns and rows for the LCD
  lcd.begin(16, 2);

  // Clear the LCD screen
  lcd.clear();
  
  // Attach the servo motor to pin 3
  myservo.attach(servoPin);

  // Initialize the Robojax_L298N_DC_motor object
  robot.begin();
  
  // Set smoke sensor pin as INPUT
  pinMode(smoke, INPUT);

  // Initialize Bluetooth communication with the given device name
  SerialBT.begin(device_name);
}

void loop() {
  // Check for available data from Bluetooth
  if (SerialBT.available()) {
    char command = SerialBT.read(); // Read the character once

    // Process the received command based on autonomous mode
    if (command == 'S' && autonomousMode) {
      autonomousMode = false;

      Serial.println("\nSwitch to Manual mode...\n");

      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Manual Mode");
    }
    else if (command == 'F') {
      Serial.println("Move Forward");

      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Move Forward");

      forward();
      delay(1000);
      brakes();
    }
    else if (command == 'B') {
      Serial.println("Move Backward");

      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Move Backward");

      backward();
      delay(1000);
      brakes();
    }
    else if (command == 'R') {
      Serial.println("Move Right");

      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Move Right");

      right();
      delay(1000);
      brakes();
    }
    else if (command == 'L') {
      Serial.println("Move Left");

      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Move Left");

      left();
      delay(1000);
      brakes();
    }
    else if (command == 'W') {
      Serial.println("LED ON");

      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("LED ON");

      digitalWrite(LED, HIGH);
    }
    else if (command == 'w') {
      Serial.println("LED OFF");

      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("LED OFF");

      digitalWrite(LED, LOW);
    }
    else if (command == 'V') {
      Serial.println("Buzzer ON");

      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Buzzer ON");

      digitalWrite(buzzer, HIGH);
    }
    else if (command == 'v') {
      Serial.println("Buzzer OFF");

      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Buzzer OFF");

      digitalWrite(buzzer, LOW);
    }
    else if (command == 'X') {
      Serial.println("Scanning the surroundings..");

      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Scanning");

      Scan();

      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Scanned");
    }
    else if (command == 'D') {
      autonomousMode = true;

      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Auto mode");

      delay(1000);

      Serial.println("\nSwitch to Autonomous mode...\n");
    }
  }
  // Perform actions in autonomous mode
  if (autonomousMode) {
    // Call the forward() function to make the robot move forward continuously
    forward();
    Scan();         // Perform scanning with the servo motor
    delay(1000);
  }
}

// Function to move the robot forward
void forward() {
  robot.rotate(motor1, 35, CW); //run motor1 at 30% speed in CW direction
  robot.rotate(motor2, 35, CW); //run motor2 at 30% speed in CW direction
}

// Function to move the robot backward
void backward() {
  robot.rotate(motor1, 35, CCW); //run motor1 at 30% speed in CCW direction
  robot.rotate(motor2, 35, CCW); //run motor2 at 30% speed in CCW direction
}

// Function to turn the robot right
void right() {
  robot.rotate(motor1, 35, CCW); //run motor1 at 30% speed in CW direction
  robot.rotate(motor2, 35, CW); //run motor1 at 30% speed in CW direction
}

// Function to turn the robot left
void left() {
  robot.rotate(motor1, 35, CW); //run motor1 at 30% speed in CW direction
  robot.rotate(motor2, 35, CCW); //run motor1 at 30% speed in CW direction
}

void brakes(){
  robot.brake(1);
  robot.brake(2); 

  delay(2000);
}

// Function to scan the environment using the servo motor
void Scan(){
  for (pos = 0; pos <= 60; pos += 10) { // goes from 0 degrees to 60 degrees
		// in steps of 10 degrees
		myservo.write(pos);    // tell servo to go to position in variable 'pos'
    allSensorReading();
    SerialMonitor();
    lcdDisplay();
    delay(100);
	}
	for (pos = 60; pos >= 0; pos -= 10) { // goes from 60 degrees to 0 degrees
		myservo.write(pos);    // tell servo to go to position in variable 'pos'
    allSensorReading();
    SerialMonitor();
    lcdDisplay();
    delay(100);
	}
}

// Function to read data from all sensors
void allSensorReading(){
  /* Flame Sensor */
  // Read the status of the flame sensor (0 for flame detected, 1 for no flame detected)
  flameReadings = analogRead(flame_sensor);
  flame_detected = map(flameReadings,  0, 4095, 0, 1);
  flameCheck();

  /* Temperature and Humidity Sensor */
  // Read temperature and humidity data from the AHT10/AHT20 sensor
  aht.getEvent(&humidity, &temp);  // Populate temp and humidity objects with fresh data
  temperatureAndHumidityCheck();

  /* Ultrasonic Sensor */
  // Send a pulse to the trigPin to trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the pulse from the echoPin
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters using the speed of sound (343 m/s) and the time it takes for the echo
  distance = duration * 0.0343 / 2;
  delay(100);
  objectCheck();

  /* Smoke Sensor */
  // Read smoke sensor data
  smoke_reading = analogRead(smoke);
  smokeCheck();

  delay(1000);
}

// Function to display data on the serial monitor
void SerialMonitor(){
  Serial.print(count++); 
  Serial.println("--------------------------------\n"); 
  // Print temperature and humidity data to the serial monitor
  Serial.print("Temperature: "); 
  Serial.print(temp.temperature); 
  Serial.println(" degrees C");

  Serial.print("Humidity: "); 
  Serial.print(humidity.relative_humidity); 
  Serial.println("% rH");

  // Print smoke sensor reading to the serial monitor
  Serial.print("Smoke sensor reading is: ");
  Serial.println(smoke_reading);
}

// Function to display data on the LCD
void lcdDisplay(){
  // Clear the LCD screen
  lcd.clear();

  // Display temperature and humidity data on the LCD
  lcd.setCursor(0, 0);
  lcd.print("T=");
  lcd.print(temp.temperature);
  lcd.write(223);

  lcd.setCursor(8, 0);
  lcd.print("H=");
  lcd.print(humidity.relative_humidity);
  lcd.print("%");

  // Display ultrasonic sensor reading on the LCD
  lcd.setCursor(8, 1);
  lcd.print("D=");
  lcd.print(distance);
  lcd.print("cm");

  // Display smoke sensor reading on the LCD
  lcd.setCursor(0, 1);
  lcd.print("S=");
  lcd.print(smoke_reading);
}

// Function to check for objects using the ultrasonic sensor
void objectCheck(){

  // Check if an object is within the proximity threshold
  if (distance < proximityThreshold) {
    // Object detected!
    Serial.print("Object Detected!! : ");
    Serial.println(distance);

    // Clear the LCD screen
    lcd.clear();

    // Print a message on the LCD screen to indicate warning
    lcd.setCursor(3, 0);
    lcd.print("Warning !!");

    lcd.setCursor(5, 1);
    lcd.print("D=");
    lcd.print(distance);
    lcd.print("cm");

    // Turn on the LED and buzzer
    digitalWrite(LED, HIGH);
    digitalWrite(buzzer, HIGH);

    delay(1000);

    if(autonomousMode){
      right(); // Perform a right turn
    
      delay(2000);

      brakes();
    }
  } else {
    // No object nearby
    Serial.print("No Nearby Object : ");
    Serial.println(distance);

    // Turn off the LED and buzzer
    digitalWrite(LED, LOW);
    digitalWrite(buzzer, LOW);
  }

}

// Function to check for flame using the flame sensor
void flameCheck(){

  // Check if a flame is detected
  if (flame_detected == 0) {
    // If flame is detected, perform the following actions
    Serial.println("Flame detected...! Take action immediately.");

    // Clear the LCD screen
    lcd.clear();

    // Print a message on the LCD screen to indicate warning
    lcd.setCursor(3, 0);
    lcd.print("Warning !!");

    lcd.setCursor(5, 1);
    lcd.print("Flame");

    digitalWrite(buzzer, HIGH);  // Turn ON the buzzer
    digitalWrite(LED, HIGH);     // Turn ON the LED

    brakes();
  } else {
    // If no flame is detected, perform the following actions
    Serial.println("No flame detected. Stay cool.");

    digitalWrite(buzzer, LOW);   // Turn OFF the buzzer
    digitalWrite(LED, LOW);      // Turn OFF the LED
  }

}

// Function to check for smoke using the smoke sensor
void smokeCheck(){

  // Check if the smoke reading is above the threshold
  if (smoke_reading >= sensor_threshold) {
    Serial.println("Smoke detected !!\n");

    // Clear the LCD screen
    lcd.clear();

    // Print a message on the LCD screen to indicate warning
    lcd.setCursor(3, 0);
    lcd.print("Warning !!");

    lcd.setCursor(5, 1);
    lcd.print("S=");
    lcd.print(smoke_reading);

    digitalWrite(buzzer, HIGH); // Turn ON the buzzer
    digitalWrite(LED, HIGH);    // Turn ON the LED

    brakes();
  } else {
    Serial.println("No Smoke...\n");

    digitalWrite(buzzer, LOW);  // Turn OFF the buzzer
    digitalWrite(LED, LOW);     // Turn OFF the LED
  }
}

// Function to check for high temperature and humidity
void temperatureAndHumidityCheck(){

    // Check if the temperature is above 32 degrees Celsius
  if (temp.temperature > 32) {
    Serial.println("ALARM !! High Temperature");

    // Clear the LCD screen
    lcd.clear();

    // Print a message on the LCD screen to indicate warning
    lcd.setCursor(3, 0);
    lcd.print("Warning !!");

    lcd.setCursor(4, 1);
    lcd.print("T=");
    lcd.print(temp.temperature);
    lcd.write(223);

    digitalWrite(LED, HIGH);     // Turn ON the LED
    digitalWrite(buzzer, HIGH);  // Turn ON the buzzer

    brakes();
  } else {
    Serial.println("Normal Temperature");

    digitalWrite(LED, LOW);      // Turn OFF the LED
    digitalWrite(buzzer, LOW);   // Turn OFF the buzzer
  }
}