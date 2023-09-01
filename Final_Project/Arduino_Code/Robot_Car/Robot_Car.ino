#include <FirebaseESP32.h>
#include <WiFiClientSecure.h>
// Firebase configuration

#define FIREBASE_HOST "https://esp-32-2c84d-default-rtdb.firebaseio.com/z"   // RealTime Database Host Link
#define FIREBASE_AUTH "985113887683"

// WiFi credentials
const char *wifi_name = "Mi 10T";
const char *password = "262004321";

String position1_path = "UsersData/Garage/Position-1";
String position2_path = "UsersData/Garage/Position-2";
String position3_path = "UsersData/Garage/Position-3";
String position4_path = "UsersData/Garage/Position-4";

String parking_position_path = "UsersData/Robot/Parking_Position";

String parking_paths[] = {position1_path, position2_path, position3_path, position4_path};

// Array to track slot occupancy
bool slotOccupied[4] = {false, false, false, false};
int park_pos;
int lafa = 0;
FirebaseData firebaseData;

//-------------------------------------------------------------------------------------


const int motor1Pin1 = 18; // IN1 Pin of L298N for Motor 1
const int motor1Pin2 = 5; // IN2 Pin of L298N for Motor 1
const int motor2Pin2 = 22; // IN1 Pin of L298N for Motor 2 (Different pin)
const int motor2Pin1 = 23; // IN2 Pin of L298N for Motor 2 (Different pin)
const int enablePin1 = 19;  // ENA Pin of L298N for Motor 1
const int enablePin2 = 4; // ENB Pin of L298N for Motor 2 (Different pin)
const int motor3Pin1 = 14; // IN1 Pin of L298N for Motor 1
const int motor3Pin2 = 27; // IN2 Pin of L298N for Motor 1
const int motor4Pin2 = 25; // IN1 Pin of L298N for Motor 2 (Different pin)
const int motor4Pin1 = 26; // IN2 Pin of L298N for Motor 2 (Different pin)
const int enablePin3 = 12;  // ENA Pin of L298N for Motor 1
const int enablePin4 = 33; // ENB Pin of L298N for Motor 2 (Different pin)

const int LEFT_PIN = 32;     // Use any 3 digital pins
const int CENTER_PIN = 35;
const int RIGHT_PIN = 34;
int leftSensor = 0;
int centerSensor = 0;
int rightSensor = 0;

const int trigPin = 15;
const int echoPin = 13;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
long duration;
float distanceCm;

void setup() {
  Serial.begin(115200);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enablePin1, OUTPUT);
  pinMode(enablePin2, OUTPUT);
  pinMode(motor3Pin1, OUTPUT);
  pinMode(motor3Pin2, OUTPUT);
  pinMode(motor4Pin1, OUTPUT);
  pinMode(motor4Pin2, OUTPUT);
  pinMode(enablePin3, OUTPUT);
  pinMode(enablePin4, OUTPUT);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // Initially stop both motors
  motorStop();

  pinMode(LEFT_PIN, INPUT);
  pinMode(CENTER_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);

  //------------------------------------------
  WiFi.begin(wifi_name, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to Wifi!");
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //------------------------------------------
  
  read();
}
void read(){

  //Reading the position to park the robot
  if (Firebase.getInt(firebaseData, parking_position_path)) {
    park_pos = firebaseData.intData();
    Serial.print("Position: ");
    Serial.println(park_pos);
  }else{
    Serial.println("Failed to read data.");
    Serial.println(firebaseData.errorReason());
  }

  //Reading Parking Positions available
  for(int i = 0; i < 4; i++){
    if (Firebase.getBool(firebaseData, parking_paths[i])) 
    {
      bool value = firebaseData.boolData();
      Serial.print("Data: ");
      Serial.println(value);
      slotOccupied[i] = value;
      }
     else {
      Serial.println("Failed to read data.");
      Serial.println(firebaseData.errorReason());
    }
  }
}

void loop() 
{

  leftSensor = digitalRead(LEFT_PIN);      // Read the 3 sensor output pins
  centerSensor = digitalRead(CENTER_PIN);
  rightSensor = digitalRead(RIGHT_PIN);
    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  delay(10);
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  while(distanceCm<5)
  {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    delay(10);
    // Calculate the distance
    distanceCm = duration * SOUND_SPEED/2;
  
    // Prints the distance in the Serial Monitor
   // Serial.print("Distance (cm): ");
    //Serial.println(distanceCm);
    motorStop();
    Serial.println("STOP");
  } 
  if (park_pos == 4 && !slotOccupied[3])
  {
    park4();
  }
  else if (park_pos == 1 && !slotOccupied[0])
  {
    park1();
  }
   else if (park_pos == 2 && !slotOccupied[1])
  {
    park2();
  }
  else if (park_pos == 3 && !slotOccupied[2])
  {
    park3();
  }
  else
  {
    motorStop();
  }
}
void park1()
{
  if (centerSensor == HIGH && leftSensor==LOW && rightSensor==LOW ||centerSensor == LOW && leftSensor==LOW && rightSensor==LOW ) 
  { 
    motorForward();
    Serial.println("Move Forward");
  }
  else if(centerSensor == HIGH && leftSensor==HIGH && rightSensor==HIGH )
  {
    delay(80);
    motorTurn2(60);
    Serial.println("Move right");
    delay(2000);
  }
  else if (centerSensor == LOW && leftSensor==HIGH && rightSensor==LOW|| centerSensor == HIGH && leftSensor==HIGH && rightSensor==LOW)
  { 
    delay(50);
    motorTurn1(70);
  }
  else if (centerSensor == LOW && leftSensor==LOW && rightSensor==HIGH || centerSensor == LOW && leftSensor==LOW && rightSensor==HIGH)
  { 
    delay(50);
    motorTurn2(70);
  }
}
void park2()
{
  if (centerSensor == HIGH && leftSensor==LOW && rightSensor==LOW ||centerSensor == LOW && leftSensor==LOW && rightSensor==LOW ) 
  { 
    motorForward();
    Serial.println("Move Forward");
  }
  else if(centerSensor == HIGH && leftSensor==HIGH && rightSensor==HIGH )
  {
    if (lafa == 0)
    {
      lafa++;
      delay(1000);
    }
    else
    {
      motorStop();
      delay(100);
      motorTurn2(80);
      Serial.println("Move right");
      delay(2000);
      motorForward();
      delay(2000);
      lafa --;
    }
  }
  else if (centerSensor == LOW && leftSensor==HIGH && rightSensor==LOW|| centerSensor == HIGH && leftSensor==HIGH && rightSensor==LOW)
  { 
    delay(50);
    motorTurn1(75);
  }
  else if (centerSensor == LOW && leftSensor==LOW && rightSensor==HIGH || centerSensor == LOW && leftSensor==LOW && rightSensor==HIGH)
  { 
    delay(50);
    motorTurn2(75);
  }
}
void park3()
{
  if (centerSensor == HIGH && leftSensor==LOW && rightSensor==LOW ||centerSensor == LOW && leftSensor==LOW && rightSensor==LOW ) 
  { 
    motorForward();
    Serial.println("Move Forward");
  }
  else if(centerSensor == HIGH && leftSensor==HIGH && rightSensor==HIGH )
  {
    if (lafa == 0)
    {
      lafa++;
      delay(1000);
    }
    else
    {
      motorStop();
      delay(100);
      motorTurn1(80);
      Serial.println("Move right");
      delay(2000);
      motorForward();
      delay(2000);
      lafa --;
    }
  }
  else if (centerSensor == LOW && leftSensor==HIGH && rightSensor==LOW|| centerSensor == HIGH && leftSensor==HIGH && rightSensor==LOW)
  { 
    delay(50);
    motorTurn1(75);
  }
  else if (centerSensor == LOW && leftSensor==LOW && rightSensor==HIGH || centerSensor == LOW && leftSensor==LOW && rightSensor==HIGH)
  { 
    delay(50);
    motorTurn2(75);
  }
}
void park4()
{
  if (centerSensor == HIGH && leftSensor==LOW && rightSensor==LOW ||centerSensor == LOW && leftSensor==LOW && rightSensor==LOW ) 
  { 
    motorForward();
    Serial.println("Move Forward");
  }
  else if(centerSensor == HIGH && leftSensor==HIGH && rightSensor==HIGH )
  {
    delay(80);
    motorTurn1(70);
    Serial.println("Move Left");
    delay(2000);
  }
  else if (centerSensor == LOW && leftSensor==HIGH && rightSensor==LOW|| centerSensor == HIGH && leftSensor==HIGH && rightSensor==LOW)
  { 
    delay(50);
    motorTurn1(70);
  }
  else if (centerSensor == LOW && leftSensor==LOW && rightSensor==HIGH || centerSensor == LOW && leftSensor==LOW && rightSensor==HIGH)
  { 
    delay(50);
    motorTurn2(70);
  }
}

// Functions to control motors and parking maneuvers

  void motorBackward() 
  {  // Control motors to move backward

    analogWrite(enablePin1, 55); // Enable Motor 1 driver
    analogWrite(enablePin2, 55); // Enable Motor 2 driver
    analogWrite(enablePin3, 55); // Enable Motor 1 driver
    analogWrite(enablePin4, 55); // Enable Motor 2 driver

    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    digitalWrite(motor3Pin1, HIGH);
    digitalWrite(motor3Pin2, LOW);
    digitalWrite(motor4Pin1, HIGH);
    digitalWrite(motor4Pin2, LOW);

  }

void motorForward() 
{   // Control motors to move forward


  analogWrite(enablePin1, 55); // Enable Motor 1 driver
  analogWrite(enablePin2, 55); // Enable Motor 2 driver
  analogWrite(enablePin3, 55); // Enable Motor 1 driver
  analogWrite(enablePin4, 55); // Enable Motor 2 driver

  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, HIGH);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2, HIGH);
}

void motorTurn1(int speed) 
{  // Control motors to move forward

  analogWrite(enablePin1, speed); // Enable Motor 1 driver
  analogWrite(enablePin2, speed); // Enable Motor 2 driver
  analogWrite(enablePin3, speed); // Enable Motor 1 driver
  analogWrite(enablePin4, speed); // Enable Motor 2 driver
  
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, HIGH);
  digitalWrite(motor4Pin1, HIGH);
  digitalWrite(motor4Pin2, LOW);
}

void motorTurn2(int speed) {
    // Control motors to turn in the other direction

  analogWrite(enablePin1, speed); // Enable Motor 1 driver
  analogWrite(enablePin2, speed); // Enable Motor 2 driver
  analogWrite(enablePin3, speed); // Enable Motor 1 driver
  analogWrite(enablePin4, speed); // Enable Motor 2 driver

  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  digitalWrite(motor3Pin1, HIGH);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2, HIGH);
}

void motorStop() 
{
  // Stop both motors
  analogWrite(enablePin1, 55); // Enable Motor 1 driver
  analogWrite(enablePin2, 55); // Enable Motor 2 driver
  analogWrite(enablePin3, 55); // Enable Motor 1 driver
  analogWrite(enablePin4, 55); // Enable Motor 2 driver

  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2,LOW);
  digitalWrite(motor3Pin1, LOW);
  digitalWrite(motor3Pin2, LOW);
  digitalWrite(motor4Pin1, LOW);
  digitalWrite(motor4Pin2,LOW);
}