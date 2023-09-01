#include <Keypad.h>     // Include the Keypad library for reading input from a matrix keypad
#include <ESP32Servo.h> // Include the ESP32Servo library to control the servo motor

#define d 50                  // Define the delay time (in milliseconds) between each servo movement
#define LINE Serial.println("--------------------------------------------") // Utility macro for printing a separator line
const byte ROWS = 4;          // Define the number of rows in the keypad
const byte COLS = 4;          // Define the number of columns in the keypad

char keys[ROWS][COLS] = { {'1','2','3','A'}, // Define the characters corresponding to each button on the keypad
                          {'4','5','6','B'}, 
                          {'7','8','9','C'}, 
                          {'*','0','#','D'} };

byte rowPins[ROWS] = { 13 , 12 , 14 , 27 }; // Define the GPIO pins connected to the keypad rows
byte colPins[COLS] = { 26 , 25 , 33 , 32 }; // Define the GPIO pins connected to the keypad columns

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS ); // Create a Keypad object

Servo servomotor;            // Create a Servo object to control the servo motor

int i = 0, pos = 0;          // Variables for storing keypad input and parsing values
int startAngle = 0, endAngle = 0; // Variables for storing the start and end angles for the servo rotation
bool stopRotation = false;   // Flag to stop the servo rotation

// Function to perform the servo rotation
void rotate(int stepDelay) {
  endAngle = constrain(endAngle, 0, 180); // Ensure the end angle is within the valid range (0-180 degrees)

  // Determine the direction of rotation (clockwise or counterclockwise)
  int direction = (startAngle < endAngle) ? 1 : -1;

  // Loop to incrementally move the servo motor from the start angle to the end angle
  loop_start:
  for (; startAngle != endAngle; startAngle += direction) {
    servomotor.write(startAngle); // Move the servo to the current angle
    delay(stepDelay);             // Wait for the servo to reach its position

    checkStopRotation(); // Check if the stop command ('*') has been received

    if (stopRotation) {
      break; // Break the loop if the stop command is received
    }
  }
  endAngle = startAngle; // Set the end angle to the current position
  stopRotation = false;  // Reset the stop flag for the next rotation
}

// Function to check if the stop command has been received
void checkStopRotation() {
  char key = keypad.getKey(); // Read a character from the keypad
  if (key == '*') {
    Serial.println("STOP!!!"); // Print a message indicating the rotation has been stopped
    Serial.print("start angle -> ");
    Serial.println(startAngle); // Print the current start angle
    Serial.print("end angle -> ");
    Serial.println(endAngle);   // Print the current end angle
    LINE;                       // Print a separator line
    stopRotation = true;        // Set the flag to stop the rotation
  }
}

void setup() {
  Serial.begin(9600); // Start serial communication with a baud rate of 9600

  servomotor.attach(5); // Attach the servo motor to the specified GPIO pin (pin 5)
  servomotor.write(0);  // Move the servo motor to its initial position (0 degrees)
  delay(50);

  Serial.println("U can use Keypad or Serial monitor"); // Print a message indicating input options
}

void loop() {
  // Check if input is available from the Serial monitor
  if (Serial.available()) {
    endAngle = Serial.parseInt(); // Read the input value from the Serial monitor
    if (Serial.read() == '\n') {
      Serial.print("Received value: ");
      Serial.println(endAngle);   // Print the received value from the Serial monitor
      LINE;                       // Print a separator line
    }
  } else {
    // If no input from the Serial monitor, check for keypad input
    char key = keypad.getKey(); // Read a character from the keypad
    int value = key - 48;       // Convert the character to an integer value

    if (value >= 0 && value <= 9 && key != 'A' && i <= 3) {
      pos = (pos * 10) + value; // Construct the number from multiple keypad inputs
      i++;
    } else if (key == '#') {
      endAngle = 0;               // If '#' is pressed, set the end angle to 0 (reset)
      Serial.println("REST!!!");  // Print a message indicating the reset
      Serial.print("start angle -> ");
      Serial.println(startAngle); // Print the current start angle
      Serial.print("end angle -> ");
      Serial.println(endAngle);   // Print the current end angle
      LINE;                       // Print a separator line
    } else if (key == 'A' || i == 3) {
      endAngle = pos;             // If 'A' is pressed or the maximum input digits are reached, set the end angle to the entered value
      Serial.print("start angle -> ");
      Serial.println(startAngle); // Print the current start angle
      Serial.print("end angle -> ");
      Serial.println(endAngle);   // Print the current end angle
      LINE;                       // Print a separator line
      pos = 0, i = 0;             // Reset the variables for the next input
    } else if (key == 'B' || key == 'C' || key == 'D') {
      Serial.println("unknown keyword"); // Print a message indicating an unknown keyword
    }
  }
  rotate(d); // Perform the servo rotation with the defined delay (d)
}
