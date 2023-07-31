// Define the pins for the IR sensor, LED, and buzzer
int arr[] = {14,27,26,25};
int button = 33;

void setup() {
  // Set the LED and buzzer pins as outputs
 

  // Initialize the serial communication for debugging
  Serial.begin(9600);
  for(int i = 0;i<4;i++){
    pinMode(arr[i],OUTPUT);
  }
  pinMode(button,INPUT);
}

void loop() {
  // Read the analog value from the IR 
  
  if(digitalRead(button)==HIGH){
    for(int i = 0;i<4;i++){
      digitalWrite(arr[i],HIGH);
    }
  }
  else{
    for(int i = 0;i<4;i++){
      digitalWrite(arr[i],LOW);
    }
  }
  
}
