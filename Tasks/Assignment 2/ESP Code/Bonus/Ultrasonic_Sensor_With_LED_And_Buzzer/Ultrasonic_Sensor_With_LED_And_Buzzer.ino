// Define the pins for the ultrasonic sensor, LED, and buzzer
const int trigPin = 33;      // Digital output pin to trigger the ultrasonic sensor
const int echoPin = 32;      // Digital input pin to receive the echo from the ultrasonic sensor
const int ledPin = 2;        // Digital output pin for the LED
const int buzzerPin = 17;    // Digital output pin for the buzzer

// Define the proximity threshold value (adjust as needed)
const int proximityThreshold = 10; // Threshold distance in centimeters

void setup() {
  // Set the trigPin as an output and the echoPin as an input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set the LED and buzzer pins as outputs
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initialize the serial communication for debugging and displaying the measured distance
  Serial.begin(115200);
}

void loop() {
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

  // Print the measured distance to the serial monitor
  Serial.print(distance);
  Serial.println(" cm");

  // Check if the distance is below the threshold
  if (distance < proximityThreshold) {
    // Object detected!
    Serial.print("Object Detected !! : ");
    
    // Turn on the LED and buzzer
    digitalWrite(ledPin, HIGH);
    //digitalWrite(buzzerPin, HIGH);
  } else {
    // No object nearby
    Serial.print("No Nearby Object : ");
    
    // Turn off the LED and buzzer
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }
  
  // Add a small delay to avoid rapid triggering
  delay(100);
}
