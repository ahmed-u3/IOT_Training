const int potPin = 4;
int led_arr[] = {18,19,21,22,23};

void setup() {

  for(int i =5;i;i--){
    pinMode(led_arr[i-1],OUTPUT);
  }
  
}

// the loop function runs over and over again forever
void loop() 
{
  int potValue = analogRead(potPin);
  potValue = map(potValue, 0, 4095, 0, 5);
  for(int i =0;i<5;i++)
    {
      if (potValue>=i+1)
        digitalWrite(led_arr[i],HIGH);
      else 
        digitalWrite(led_arr[i],LOW);
    }
  }
