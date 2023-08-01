#include <ESP32Servo.h>   // Include the ESP32Servo library to control the servo motor

#define poten 25          // Define the analog pin number for the potentiometer
#define servo_pin 5       // Define the GPIO pin number for the servo motor
#define step_delay 50     // Define the delay time (in milliseconds) between each servo movement

Servo servo;             // Create a Servo object to control the servo motor

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);    // Start serial communication with a baud rate of 9600
  pinMode(poten, INPUT); // Set the potentiometer pin as input
  servo.attach(servo_pin); // Attach the servo motor to the specified GPIO pin
  servo.write(0);        // Move the servo motor to its initial position (0 degrees)
  delay(step_delay);     // Wait for the servo to reach its position before proceeding
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int signal = analogRead(poten);  // Read the analog value from the potentiometer
  Serial.print("poten value -> "); // Print a label for the potentiometer value
  Serial.println(signal);          // Print the current value read from the potentiometer
  signal = map(signal, 0, 4095, 0, 180); // Map the potentiometer value to the servo motor's angle range (0-180 degrees)
  Serial.print("Angle -> ");       // Print a label for the mapped angle value
  Serial.println(signal);          // Print the mapped angle value
  servo.write(signal);             // Move the servo motor to the mapped angle position
  delay(step_delay);               // Wait for the servo to reach its position before reading the potentiometer again
}
