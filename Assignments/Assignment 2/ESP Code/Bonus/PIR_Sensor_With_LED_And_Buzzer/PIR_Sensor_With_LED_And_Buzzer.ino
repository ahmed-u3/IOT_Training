const int pirSensorPin = 33;   // PIR sensor input pin
const int ledPin = 2;         // LED output pin
const int buzzerPin = 19;     // Buzzer output pin

//const int motionThreshold = 1;   // The assumed threshold for motion detection

void setup() {
  pinMode(pirSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int motionDetected = digitalRead(pirSensorPin);

  if (motionDetected == HIGH) {
    // Motion detected!
    Serial.println("Motion Detected!!");
    
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(500); // Buzzer ON time
    
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(500); // Buzzer OFF time
  } else {
    // No motion detected
    Serial.println("No Motion");
    
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }
}
