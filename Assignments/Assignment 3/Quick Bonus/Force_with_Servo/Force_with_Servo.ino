#include <ESP32Servo.h>

#define servo_pin 4        // Servo motor control pin
#define force_pin 34       // Force sensor analog input pin
int threshold = 1200;     // Threshold value to determine if pressure is applied

Servo myservo;            // Create a Servo object to control the servo motor

void operate(int signal) {
  if (signal > threshold) {   // If the pressure signal from the force sensor is greater than the threshold
    myservo.write(180);      // Rotate the servo motor to 180 degrees (open position)
  } else {
    myservo.write(0);        // If the pressure signal is equal to or below the threshold, rotate the servo motor to 0 degrees (closed position)
  }
}

void setup() {
  myservo.attach(servo_pin);  // Attach the servo to the specified pin
  Serial.begin(9600);         // Initialize the serial communication for debugging purposes
}

void loop() {
  int pressure = map(analogRead(force_pin), 1600, 4095, 0, 2000);  // Read the analog value from the force sensor and map it to a pressure range of 0 to 2000
  Serial.print("Pressure: ");
  Serial.println(pressure);   // Print the pressure value to the serial monitor for debugging

  operate(pressure);  // Call the function 'operate' with the pressure value from the force sensor as an argument

}
