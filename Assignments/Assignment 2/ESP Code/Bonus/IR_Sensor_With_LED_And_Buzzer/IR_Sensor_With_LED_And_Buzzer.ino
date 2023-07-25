// Define the pins for the IR sensor, LED, and buzzer
const int irSensorPin = 34;  // Analog input pin for the IR sensor
const int ledPin = 2;        // Digital output pin for the LED
const int buzzerPin = 19;    // Digital output pin for the buzzer

// Define the proximity threshold value (adjust as needed)
const int proximityThreshold = 200; 

void setup() {
  // Set the LED and buzzer pins as outputs
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize the serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Read the analog value from the IR sensor
  int proximityValue = analogRead(irSensorPin);

  // Check if the proximity value is below the threshold
  if (proximityValue < proximityThreshold) {
    // Object detected!
    Serial.println("Object Detected !!");
    
    // Turn on the LED and buzzer
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH); 
  } else {
    // No object nearby
    Serial.println("No Nearby Object");
    
    // Turn off the LED and buzzer
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }
  
  // Add a small delay to avoid rapid triggering
  delay(100); 
}
