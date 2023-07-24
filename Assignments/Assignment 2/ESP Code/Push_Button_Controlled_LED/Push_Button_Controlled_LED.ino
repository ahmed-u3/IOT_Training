int buttonPin = 23;
int led = 2;
void setup() {
  pinMode(led,OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() 
{
  int state = digitalRead(buttonPin);
  if (state == HIGH)
  {
    digitalWrite(led , LOW);
  }
  else digitalWrite(led , HIGH);

  }
