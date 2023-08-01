#include <Adafruit_AHTX0.h>

// Initialize the AHT10/AHT20 sensor object
Adafruit_AHTX0 aht;

// Define the pins for the LED and the buzzer
int led = 2;        /* LED output Pin */
int buzzer = 19;    /* Buzzer output Pin */

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Adafruit AHT10/AHT20 demo!");

  // Check if the AHT10/AHT20 sensor is connected and functioning
  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10); // Hangs the program in an infinite loop if sensor not found
  }
  Serial.println("AHT10 or AHT20 found");

  // Set the mode for LED and buzzer pins as OUTPUT
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {
  // Variables to store temperature and humidity data
  sensors_event_t humidity, temp;

  // Get fresh temperature and humidity data from the AHT10/AHT20 sensor
  aht.getEvent(&humidity, &temp);

  // Print temperature and humidity data to the serial monitor
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
  Serial.println();

  // Check if the temperature is above 32 degrees Celsius
  if (temp.temperature > 32) {
    Serial.println("ALARM !! High Temperature");

    // Turn ON the LED and the buzzer
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
  } else {
    Serial.println("Normal Temperature");

    // Turn OFF the LED and the buzzer
    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);
  }

  // Add a delay to avoid excessive readings and to allow the user to read the data
  delay(500);
}
