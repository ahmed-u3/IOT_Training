// Include necessary libraries
#include <LiquidCrystal.h>   // Library for the LCD display
#include <ESP32Servo.h>      // Library for controlling Servo motors
#include <Adafruit_AHTX0.h>  // Library for the AHT10/AHT20 temperature and humidity sensor

// Initialize objects for AHT10/AHT20 sensor and Servo motor
Adafruit_AHTX0 aht;
Servo myservo;

// Define the pins used for various components
const int trigPin = 8;      // Digital output pin to trigger the ultrasonic sensor
const int echoPin = 9;      // Digital input pin to receive the echo from the ultrasonic sensor

int buzzer = 3;             // Pin connected to the buzzer
int LED = 2;                // Pin connected to the LED

int flame_sensor = 13;      // Pin connected to the flame sensor

int flame_detected;         // Variable to store the flame detection status

// Define the proximity threshold value (adjust as needed)
const int proximityThreshold = 10; // Threshold distance in centimeters

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize AHT10/AHT20 sensor
  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);  // Hangs the program in an infinite loop
  }
  Serial.println("AHT10 or AHT20 found");

  // Set the mode for LED and buzzer pins as OUTPUT
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Set the trigPin as an OUTPUT and the echoPin as an INPUT
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set the flame sensor pin as an INPUT
  pinMode(flame_sensor, INPUT);

  // Initialize the LCD module with the specified pin configuration
  const int rs = 22, en = 23, d4 = 5, d5 = 18, d6 = 19, d7 = 21;
  LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

  // Set up the number of columns and rows for the LCD
  lcd.begin(16, 2);

  // Clear the LCD screen
  lcd.clear();

  // Attach the servo motor to pin 3
  myservo.attach(3);
}

void loop() {
  // Read the status of the flame sensor (0 for flame detected, 1 for no flame detected)
  flame_detected = digitalRead(flame_sensor);

  // Read temperature and humidity data from the AHT10/AHT20 sensor
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);  // Populate temp and humidity objects with fresh data

  // Print temperature and humidity data to the serial monitor
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");

  // Display temperature and humidity data on the LCD
  lcd.setCursor(0, 1);
  lcd.print("T=");
  lcd.print(temp.temperature);
  lcd.print("°");

  lcd.setCursor(8, 0);
  lcd.print("H=");
  lcd.print(humidity.relative_humidity);
  lcd.print("%");

  // Send a pulse to the trigPin to trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the pulse from the echoPin
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters using the speed of sound (343 m/s) and the time it takes for the echo
  int distance = duration * 0.0343 / 2;

  // Check if a flame is detected
  if (flame_detected == 0) {
    // If flame is detected, perform the following actions
    Serial.println("Flame detected...! Take action immediately.");

    // Print a message on the LCD screen to indicate warning
    lcd.setCursor(7, 1);
    lcd.print("Warning !!");

    digitalWrite(buzzer, HIGH);  // Turn ON the buzzer
    digitalWrite(LED, HIGH);     // Turn ON the LED

    delay(1000);
  } else {
    // If no flame is detected, perform the following actions
    Serial.println("No flame detected. Stay cool.");

    // Print a message on the LCD screen to indicate normal
    lcd.setCursor(10, 1);
    lcd.print("Normal");

    digitalWrite(buzzer, LOW);   // Turn OFF the buzzer
    digitalWrite(LED, LOW);      // Turn OFF the LED
  }

  // Check if the temperature is above 32 degrees Celsius
  if (temp.temperature > 32) {
    Serial.println("ALARM !! High Temperature");

    // Print a message on the LCD screen to indicate warning
    lcd.setCursor(7, 1);
    lcd.print("Warning !!");

    digitalWrite(LED, HIGH);     // Turn ON the LED
    digitalWrite(buzzer, HIGH);  // Turn ON the buzzer

    delay(1000);
  } else {
    Serial.println("Normal Temperature");

    lcd.setCursor(10, 1);
    lcd.print("Normal");

    digitalWrite(LED, LOW);      // Turn OFF the LED
    digitalWrite(buzzer, LOW);   // Turn OFF the buzzer
  }

  // Check if an object is within the proximity threshold
  if (distance < proximityThreshold) {
    // Object detected!
    Serial.print("Object Detected !! : ");

    // Print a message on the LCD screen to indicate warning
    lcd.setCursor(7, 1);
    lcd.print("Warning !!");

    // Turn on the LED and buzzer
    digitalWrite(LED, HIGH);
    digitalWrite(buzzer, HIGH);

    delay(1000);
  } else {
    // No object nearby
    Serial.print("No Nearby Object : ");

    lcd.setCursor(10, 1);
    lcd.print("Normal");

    lcd.setCursor(0, 1);
    lcd.print("D=");
    lcd.print(distance);
    lcd.print("cm");

    // Turn off the LED and buzzer
    digitalWrite(LED, LOW);
    digitalWrite(buzzer, LOW);
  }

  // Print the measured distance to the serial monitor
  Serial.print(distance);
  Serial.println(" cm");

  // Map the distance to an angle for the servo motor
  int pos = map(distance, 5, 40, 0, 180);
  myservo.write(pos);

  // Delay before the next iteration
  delay(3000);
}
