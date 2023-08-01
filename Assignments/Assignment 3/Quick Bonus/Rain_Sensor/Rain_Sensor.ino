#include<ESP32Servo.h> // Include the ESP32Servo library to control the servo motor

#define led 23          // Define the GPIO pin number for the LED
#define rain_sensor 35  // Define the analog pin number for the rain sensor
#define servo_pin 5     // Define the GPIO pin number for the servo motor

Servo servo;            // Create a Servo object to control the servo motor

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   // Start serial communication with a baud rate of 9600
  pinMode(led, OUTPUT); // Set the LED pin as output
  servo.attach(servo_pin); // Attach the servo motor to the specified GPIO pin
  servo.write(0);       // Move the servo motor to its initial position (0 degrees)
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = analogRead(rain_sensor); // Read the analog value from the rain sensor
  Serial.print("Value -> "); // Print a label for the rain sensor value
  Serial.println(value);     // Print the current value read from the rain sensor

  if (value < 1500) {
    digitalWrite(led, HIGH); // Turn on the LED if the rain sensor value is less than 1500 (indicating heavy rain)
    Serial.println("Heavy rain led on"); // Print a message indicating heavy rain status
    servo.write(180);     // Move the servo motor to its maximum angle position (180 degrees)
    delay(50);            // Add a delay to avoid rapid servo movements
  } else {
    digitalWrite(led, LOW); // Turn off the LED if the rain sensor value is greater than or equal to 1500
    servo.write(0);         // Move the servo motor to its minimum angle position (0 degrees)
    delay(50);              // Add a delay to avoid rapid servo movements
  }
}
