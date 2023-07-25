
void setup() 
{

  // Initialize the LED pins as OUTPUT in a loop
  for (int i = 5; i; i--) {
    pinMode(led_arr[i - 1], OUTPUT);
  }
}

void loop() {

  // Read the analog value from the potentiometer
  int potValue = analogRead(potPin);

  // Map the potentiometer value to a range of 0 to 5
  potValue = map(potValue, 0, 4095, 0, 5);

  // Loop through each LED
  for (int i = 0; i < 5; i++) {

    // Check if the potValue is greater than or equal to (i + 1)
    if (potValue >= i + 1) {
      // If true, turn on the LED by setting its pin to HIGH
      digitalWrite(led_arr[i], HIGH);
    } else {
      // If false, turn off the LED by setting its pin to LOW
      digitalWrite(led_arr[i], LOW);
    }
  }
}
