// Define the pins used for the components
int buzzer = 22;         // Pin connected to the buzzer
int LED = 2;             // Pin connected to the LED
int flame_sensor = 13;   // Pin connected to the flame sensor

int flame_detected;      // Variable to store the flame detection status

void setup()
{
  Serial.begin(9600);    // Start serial communication at 9600 bps
  pinMode(buzzer, OUTPUT);   // Set the buzzer pin as an OUTPUT
  pinMode(LED, OUTPUT);       // Set the LED pin as an OUTPUT
  pinMode(flame_sensor, INPUT);  // Set the flame sensor pin as an INPUT
}

void loop()
{
  // Read the status of the flame sensor (0 for flame detected, 1 for no flame detected)
  flame_detected = digitalRead(flame_sensor);

  if (flame_detected == 0)
  {
    // If flame is detected, perform the following actions
    Serial.println("Flame detected...! Take action immediately.");
    digitalWrite(buzzer, HIGH); // Turn ON the buzzer
    digitalWrite(LED, HIGH);    // Turn ON the LED
    delay(500);                 // Wait for 500 milliseconds
    digitalWrite(LED, LOW);     // Turn OFF the LED
    delay(500);                 // Wait for another 500 milliseconds
  }
  else
  {
    // If no flame is detected, perform the following actions
    Serial.println("No flame detected. Stay cool.");
    digitalWrite(buzzer, LOW);  // Turn OFF the buzzer
    digitalWrite(LED, LOW);     // Turn OFF the LED
  }

  delay(1000); // Wait for 1 second before repeating the loop
}
