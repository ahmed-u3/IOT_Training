#include <LiquidCrystal.h> // Include the LiquidCrystal library for controlling the LCD display
#include <IRremote.h>      // Include the IRremote library for handling infrared (IR) remote signals

#define IRSensor A0        // Define the analog pin number where the IR sensor module is connected
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // Define the LCD pins

IRrecv irReceiver(IRSensor); // Create an IRrecv object to receive IR signals
decode_results irResults;   // Variable to store the decoded IR signal data

LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // Create a LiquidCrystal object to control the LCD

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2); // Initialize the LCD with 16 columns and 2 rows
  Serial.begin(9600); // Start serial communication with a baud rate of 9600
  
  irReceiver.enableIRIn(); // Enable the IR receiver to receive incoming IR signals
}

void loop() {
  int sensorStatus = analogRead(IRSensor); // Read the analog value from the IR sensor

  if (sensorStatus != 0) { // If the IR sensor is detecting a signal (not zero)
    lcd.clear(); // Clear the LCD screen
    lcd.print("Distance: "); // Print the label "Distance:"
    lcd.setCursor(0, 1); // Set the cursor to the first column of the second row
    lcd.print(sensorStatus); // Print the analog value from the IR sensor on the LCD
    Serial.println(sensorStatus); // Print the analog value to the serial monitor
    irReceiver.resume(); // Resume receiving IR signals in case an IR remote signal was detected

  } else { // If the IR sensor is not detecting any signal (value is zero)
    lcd.clear(); // Clear the LCD screen
    lcd.print("IR sensor removed"); // Print a message indicating that the IR sensor is not detected
    Serial.println("IR sensor removed"); // Print the same message to the serial monitor
  }

  delay(1000); // Wait for one second before updating the LCD and reading the IR sensor again
}
