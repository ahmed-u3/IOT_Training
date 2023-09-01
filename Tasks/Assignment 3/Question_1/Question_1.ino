#include <ESP32Servo.h>

#define ir_pin 34         // IR sensor input pin
#define servo_pin 4       // Servo motor control pin
#define threshold 600     // Threshold value to determine if an object is detected

Servo myservo;           // Create a Servo object to control the servo motor

void operate(int signal){

  if(signal < threshold){   // If the analog signal from the IR sensor is below the threshold value
    myservo.write(180);    // Rotate the servo motor to 180 degrees (open position)
    delay(3000);           // Wait for 3 seconds (servo remains open for this duration)
  } else {
    myservo.write(0);      // If the analog signal is equal to or above the threshold, rotate the servo motor to 0 degrees (closed position)
  }
}

void setup() {
  Serial.begin(9600);     // Initialize the serial communication for debugging purposes
  myservo.attach(servo_pin);  // Attach the servo to the specified pin

  // Setup is used for one-time initialization tasks and runs only once when the board starts up.
}

void loop() {
  // loop is the main function that runs repeatedly after setup.

  int signal = analogRead(ir_pin);  // Read the analog value from the IR sensor
  Serial.print("Signal: ");
  Serial.println(signal);          // Print the analog value to the serial monitor for debugging

  operate(signal);    // Call the function 'operate' with the analog value from the IR sensor as an argument
}
