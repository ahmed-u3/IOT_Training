#include <Keypad.h>// Library for Keypad
#include <Servo.h>// Library for Servo
#include <LiquidCrystal_I2C.h> // Library for LCD

Servo myservo;  // create servo object to control a servo

// Define keypad dimensions and pins
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

// Define pin numbers for various sensors and components
const int ldrPin = A0; // analog pin 0
const int ledanbuzzer = 13;
const int IRPin = A1;

// Global variables
String pad; // Stores entered keypad input
char key;   // Stores current key pressed

// Define keypad layout
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Define keypad row and column pin connections
byte rowPins[ROWS] = {12, 11, 10, 9};
byte colPins[COLS] = {5, 4, 3, 2};

// Create an object of the Keypad library
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Initialize LCD using I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Function to perform sweeping motion of the servo
void swip() {
  int pos = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Turn to 180");
  
  // Sweep servo from 0 to 180 degrees
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(10);
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Back to 0");
  
  // Sweep servo from 180 to 0 degrees
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(10);
  }
}

// Function to read keypad password
void readKeypad() {
  key = keypad.waitForKey();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Pass");
  
  if (key != '#') {
    String knov = String(key);
    pad += knov;
    Serial.println(pad);
    lcd.setCursor(0, 1);
    lcd.print(pad);
  }
  
  if (key == '*') {
    pad = "";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Pass");
  }
}

void setup() {
  Serial.begin(9600);
  myservo.attach(A2);
  pinMode(ldrPin, INPUT);
  pinMode(ledanbuzzer, OUTPUT);
  pinMode(A3, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
}

void loop() {
  Serial.println("Welcome");
  myservo.write(0);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Select Mode");
  lcd.setCursor(0, 1);
  lcd.print("1 2 3 4");
  
  key = keypad.waitForKey(); // Read the key
  digitalWrite(ledanbuzzer, LOW);
  
  if (key == '1') {
    // Mode 1: Read LDR and IR sensor values
    while (key != '#') {
      key = keypad.getKey();
      int ldrStatus = analogRead(ldrPin);
      Serial.print("The LDR sensor is = ");
      Serial.println(ldrStatus);
      int IRStatus = analogRead(IRPin);
      Serial.print("The IR sensor is = ");
      Serial.println(IRStatus);
      
      // Control LED and buzzer based on LDR value
      if (ldrStatus > 350) {
        digitalWrite(ledanbuzzer, HIGH);
      } else {
        digitalWrite(ledanbuzzer, LOW);
      }
      
      // Display sensor values on the LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("LDR: ");
      lcd.setCursor(5, 0);
      lcd.print(ldrStatus);
      lcd.setCursor(0, 1);
      lcd.print("IR: ");
      lcd.setCursor(5, 1);
      lcd.print(IRStatus);

      delay(2000);
    }
  }
  
  if (key == '2') {
    // Mode 2: Perform servo sweeping motion
    swip();
  }
  
  if (key == '3') {
    // Mode 3: Enter password to control servo
    Serial.println("Mode 3");
    pad = "";
    String password = "1234";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Pass");
    
    while (pad != password) {
      pad = "";
      key = "";
      
      // Read keypad input until '#' is pressed
      while (key != '#') {
        readKeypad();
      }
      
      if (pad == password) {
        Serial.println("Approve Welcome");
        myservo.write(175);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Approve Welcome");
        delay(5000);
      } else {
        Serial.println("Try Again");
        myservo.write(0);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Try Again");
        delay(1000);
      }
    }
  }
  
  if (key == '4') {
    // Mode 4: Control servo with IR sensor and button
    Serial.println("Mode 4");
    
    while (key != '#') {
      key = keypad.getKey();
      int buttonval = digitalRead(A3); // Read the value of the button connected to pin A3
      int IRValue = analogRead(A1);
      
      // Control servo based on IR sensor value and button state
      if (IRValue < 500 && buttonval == HIGH) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("IR: ");
        lcd.setCursor(5, 0);
        lcd.print(IRValue);
        myservo.write(175); // Move the servo motor to position 175 degrees
        Serial.println(IRValue);
      } else if (IRValue > 500 && buttonval == HIGH) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("IR: ");
        lcd.setCursor(5, 0);
        lcd.print(IRValue);
        myservo.write(0); // Move the servo motor to position 0 degrees
        Serial.println(IRValue);
      }
      
      delay(500);
    }
  }
}
