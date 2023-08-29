// Include necessary libraries
#include <LiquidCrystal_I2C.h>  // Library for the LCD display
#include <Servo.h>      // Library for controlling Servo motors
#include <Adafruit_AHTX0.h>  // Library for the AHT10/AHT20 temperature and humidity sensor

// Initialize objects for AHT10/AHT20 sensor and Servo motor
Adafruit_AHTX0 aht;          // Object for AHT10/AHT20 sensor
Servo myservo;               // Object for Servo motor



LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


// Pin assignments
int buzzer = 3;              // Pin connected to the buzzer
int LED = 2;                 // Pin connected to the LED
#define smoke A0             // Smoke sensor pin
#define sensor_threshold 150 // Threshold value for smoke sensor
const int pirSensorPin = 4; // PIR sensor input pin

int smoke_reading;           // Variable to store smoke sensor reading
int angle = 0;               // Variable to store servo motor angle

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Adafruit AHT10/AHT20 demo!");

  // Check if the AHT10/AHT20 sensor is connected and functioning
  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1)
      delay(10); // Hangs the program in an infinite loop if sensor not found
  }
  Serial.println("AHT10 or AHT20 found");

  // Set the mode for LED and buzzer pins as OUTPUT
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);

// initialize the lcd with I2C
   lcd.init(); 
   lcd.backlight();
   lcd.clear();



  // Attach the servo motor to pin 9
  myservo.attach(9);

  // Set smoke sensor pin as INPUT
  pinMode(smoke, INPUT);

  pinMode(pirSensorPin, INPUT);
}

void loop() {
  // Read temperature and humidity data from the AHT10/AHT20 sensor
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp); // Populate temp and humidity objects with fresh data

  // Print temperature and humidity data to the serial monitor
  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degrees C");
  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println("% rH");

  // Display temperature and humidity data on the LCD
  lcd.setCursor(0, 0);
  lcd.print("T=");
  lcd.print(temp.temperature);
  lcd.print("°");

  lcd.setCursor(6, 0);
  lcd.print("  H=");
  lcd.print(humidity.relative_humidity);
  lcd.print("%");

  // Read smoke sensor data
  smoke_reading = analogRead(smoke);

  // Print smoke sensor reading to the serial monitor
  Serial.print("Smoke sensor reading is: ");
  Serial.println(smoke_reading);

  
  int currentMotionState = digitalRead(pirSensorPin);

  // Check if smoke reading is above the threshold
  if (smoke_reading >= sensor_threshold) {
    Serial.println("Smoke detected !!");

    // Print a message on the LCD screen to indicate warning
  // Display smoke sensor reading on the LCD
  lcd.setCursor(0, 1);
  lcd.print("S=");
  lcd.print(smoke_reading);

    lcd.setCursor(7, 1);
    lcd.print("Warning!!");

    digitalWrite(buzzer, HIGH); // Turn ON the buzzer
    digitalWrite(LED, HIGH);    // Turn ON the LED

    delay(1000);
  } else {
    Serial.println("No Smoke...");

    // Print a message on the LCD screen to indicate normal
    lcd.setCursor(0, 1);
    lcd.print("                       ");  

    digitalWrite(buzzer, LOW); // Turn OFF the buzzer
    digitalWrite(LED, LOW);    // Turn OFF the LED
  }

  // Check if the temperature is above 32 degrees Celsius
  if (temp.temperature > 32.5) {
    Serial.println("ALARM !! High Temperature");

    // Print a message on the LCD screen to indicate warning
    lcd.setCursor(0, 1);
    lcd.print("Temp Warning !");

    digitalWrite(LED, HIGH);    // Turn ON the LED
    digitalWrite(buzzer, HIGH); // Turn ON the buzzer

    delay(1000);
  } else {
    Serial.println("Normal Temperature");
    
    lcd.setCursor(7, 1);
    lcd.print("              ");  

    
    digitalWrite(LED, LOW); // Turn OFF the LED
    digitalWrite(buzzer, LOW); // Turn OFF the buzzer
  }

  if (currentMotionState == HIGH) {
    Serial.println("Motion Detected!!");

    lcd.clear();
    lcd.print("Motion Detected!!");

    digitalWrite(LED, HIGH);    // Turn ON the LED
    digitalWrite(buzzer, HIGH); // Turn ON the buzzer

    delay(1000);

    if (angle < 160) {
      myservo.write(160); //open the door
      delay(3000); // waiting period
      myservo.write(0); // close the dor

    }
  } else if (currentMotionState == LOW) {
    Serial.println("No Motion");

    digitalWrite(LED, LOW);    // Turn OFF the LED
    digitalWrite(buzzer, LOW); // Turn OFF the buzzer
    if (angle != 0) {
      myservo.write(0);
    }
  }

  delay(1000);
}