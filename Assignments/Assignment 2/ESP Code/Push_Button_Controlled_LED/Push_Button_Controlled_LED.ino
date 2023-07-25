// Define the pin number for the button
int buttonPin = 23;

// Define the pin number for the LED
int led = 2;

void setup() {
  // Set the LED pin as OUTPUT
  pinMode(led, OUTPUT);

  // Set the button pin as INPUT with internal pull-up resistor enabled
  pinMode(buttonPin, INPUT_PULLUP);
}
void loop() {
  // Read the current state of the button (HIGH or LOW)
  int state = digitalRead(buttonPin);

  // Check if the button is pressed (LOW state)
  if (state == HIGH) {
    // If the button is not pressed, turn off the LED by setting its pin to LOW
    digitalWrite(led, LOW);
  } else {
    // If the button is pressed, turn on the LED by setting its pin to HIGH
    digitalWrite(led, HIGH);
  }
}
