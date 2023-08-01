#include <Servo.h> // Include the Servo library to control the servo motor

Servo myservo;   // Create a servo object to control a servo motor
int sensorPin = A0; // Select the analog input pin for the LDR (Light Dependent Resistor)
int ledPin = 12; // Define the digital output pin for the LED
int sensorValue = 0; // Variable to store the value coming from the LDR
#define IRSensor A1 // Define the digital input pin for an infrared (IR) sensor
#define potpin A3    // Define the analog input pin for a potentiometer
int potval =  0; // Variable to store the value coming from the potentiometer

void setup() {
  Serial.begin(9600); // Sets up the serial port for communication with a baud rate of 9600
  pinMode(12, OUTPUT); // Set pin 12 as an output for the LED
  pinMode(2, INPUT_PULLUP); // Set pin 2 as an input with internal pull-up resistor enabled
}

void loop() {
  sensorValue = analogRead(sensorPin); // Read the value from the LDR

  Serial.println(sensorValue); // Print the LDR value to the serial monitor

  myservo.attach(9); // Attach the servo motor to pin 9

  int buttonval = digitalRead(2); // Read the value of the button connected to pin 2

  int IRValue = digitalRead(IRSensor); // Read the value from the infrared (IR) sensor

  // Check if the IR sensor is detecting a signal (IRValue is LOW) and the button is pressed (buttonval is HIGH)
  if (IRValue == 0 && buttonval == HIGH) {
    myservo.write(175); // Move the servo motor to position 175 (degrees)
    digitalWrite(ledPin, HIGH); // Turn on the LED

  } else {
    myservo.write(0); // Move the servo motor to position 0 (degrees)
    digitalWrite(ledPin, LOW); // Turn off the LED

    // Check if the LDR value is greater than or equal to 850
    if (sensorValue >= 850) {
      myservo.write(175); // Move the servo motor to position 175 (degrees)
    } else {
      myservo.write(0); // Move the servo motor to position 0 (degrees)

      potval = analogRead(potpin); // Read the value from the potentiometer
      potval = map(potval, 0, 1023, 0, 175); // Map the potentiometer value to the servo motor's angle range (0-175 degrees)
      myservo.write(potval); // Move the servo motor to the mapped angle position
    }
  }
  delay(500); // Add a delay to control the servo motor movements and avoid rapid changes
}
