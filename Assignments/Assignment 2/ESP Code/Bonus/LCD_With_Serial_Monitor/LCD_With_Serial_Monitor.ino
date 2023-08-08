// Include the LiquidCrystal library
#include <LiquidCrystal.h>

// Initialize the LCD module
const int rs = 3, en = 23, d4 = 5, d5 = 18, d6 = 19, d7 = 16;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Setup function runs once at the beginning of the program
void setup() {
  // Set up the number of columns and rows for the LCD
  lcd.begin(16, 2);
  
  // Clear the LCD screen
  lcd.clear();
  
  // Prompt the user to enter their name
  Serial.begin(115200);
}

// Loop function runs repeatedly as long as the Arduino is powered on
void loop() {
  // Print a message to the Serial Monitor prompting the user to enter their name
  Serial.println("Enter your name:");
  
  // Run the following code indefinitely until a name is entered
  do {
    // Check if there is any input available from the Serial Monitor
    if (Serial.available()) {
      // Read the input from the Serial Monitor until a newline character is encountered
      String name = Serial.readStringUntil('\n');
    
      // Clear the LCD screen and set the cursor to the beginning of the first line
      lcd.clear();
      lcd.setCursor(0, 0);
    
      // Print the entered name on the Serial Monitor
      Serial.println(name);
      
      // Print a message on the LCD screen to indicate the user's name is displayed
      lcd.print("Your Name is : ");
      
      // Set the cursor to column 0, line 1 (second row) of the LCD screen
      lcd.setCursor(0, 1);
      
      // Print the entered name on the LCD screen
      lcd.print(name);

      // Break out of the loop since we have processed the input
      break;
    }
  } while(true); // Loop indefinitely until a name is entered
}
