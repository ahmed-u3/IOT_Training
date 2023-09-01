#include <ESP32Servo.h>
#include <Adafruit_AHTX0.h>
#include <Keypad.h>// Library for Keypad
#include <LiquidCrystal_I2C.h> // Library for LCD

//-------------------------------------------------------------------
#include <FirebaseESP32.h>
#include <WiFiClientSecure.h>

// Firebase configuration
#define FIREBASE_HOST "https://esp-32-2c84d-default-rtdb.firebaseio.com/"   // RealTime Database Host Link
#define FIREBASE_AUTH "985113887683"

const char *wifi_name = "salsola";
const char *password = "bozo2010";

String alarm_path = "UsersData/Home/Alarm";
String door_path = "UsersData/Home/Door";
String fan_path = "UsersData/Home/Fan";
String gate_path = "UsersData/Home/Gate";
String light_path = "UsersData/Home/Light";
String rain_path = "UsersData/Home/Rain";
String temp_path = "UsersData/Home/Temp";
String window_path = "UsersData/Home/Window";
String ldr_path = "UsersData/Home/LDR";
String flame_path = "UsersData/Home/Flame";
String smoke_path = "UsersData/Home/Smoke";
String ir_path = "UsersData/Home/IR";

FirebaseData firebaseData;

//---------------------------------------------------------------------

#define ForcePin 15       // Force sensor analog input pin
int Pressure;
#define PressureThreshold 500

#define LDR 33
int LDR_Readings = 0;
#define LDRThreshold 3600

#define Window 14 // Window
#define Gate 16 // Gate
#define Door 23 // door

Servo Servo_Window;
Servo Servo_Gate;
Servo Servo_Door;

bool Someone = false;

#define FlameSensor 13      // Pin connected to the flame sensor
int FlameDetected;         // Variable to store the flame detection status

#define Smoke 35  // smoke sensor pin (analog)
#define SmokeThreshold 1000 // threshold value 
int SmokeReadings;

#define LEDandBuzzer 2
#define Smoke_TONE 500
#define Flame_TONE 700
#define Brightness_TONE 1000

Adafruit_AHTX0 aht;   // Initialize the AHT10/AHT20 sensor object
// Variables to store temperature and humidity data
sensors_event_t humidity, temp;

#define IN1 18
#define IN2 19

#define IR_Pin 32         // IR sensor input pin
int IR_Readings;
bool Flag = false;

#define RainSensor 34  // Define the analog pin number for the rain sensor
int RainDropReadings;
bool Rain = false; 

// Define keypad dimensions and pins
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
// Global variables
String pad; // Stores entered keypad input
char key;   // Stores current key pressed
// Define keypad layout
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// Define keypad row and column pin connections
byte rowPins[ROWS] = {12, 17, 27, 26};
byte colPins[COLS] = {25, 4, 5, 3};
// Create an object of the Keypad library
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
// Initialize LCD using I2C
LiquidCrystal_I2C LCD(0x27, 16, 2);

// Calibration parameters for your IR sensor
const int minAnalogValue = 100;   // Analog reading at minimum distance
const int maxAnalogValue = 800;   // Analog reading at maximum distance
const int minDistanceCM = 5;      // Minimum distance in cm
const int maxDistanceCM = 30;     // Maximum distance in cm

//---------------------------------------------------------------------------------

// Function to upload integer data to Firebase
void upload_int(String path, int value){
  if (Firebase.setInt(firebaseData, path, value)) {
    Serial.println("Data sent to Firebase!");
  } else {
    Serial.println("Failed to send data to Firebase.");
    Serial.println(firebaseData.errorReason());
  }
}

// Function to upload boolean data to Firebase
void upload_bool(String path, bool value){
  
  if (Firebase.setBool(firebaseData, path, value)) {
    Serial.println("Data sent to Firebase!");
  } else {
    Serial.println("Failed to send data to Firebase.");
    Serial.println(firebaseData.errorReason());
  }

}

// Function to read boolean data from Firebase
bool read(String path){
  if (Firebase.getBool(firebaseData, path)) {
    bool value = firebaseData.boolData();
    return value;
  } else {
    Serial.println("Failed to read data.");
    Serial.println(firebaseData.errorReason());
  }
}

//---------------------------------------------------------------------------------


void setup() {
  Serial.begin(9600);

  Servo_Window.attach(Window);  
  Servo_Gate.attach(Gate);
  Servo_Door.attach(Door);

  pinMode(FlameSensor, INPUT);
  pinMode(Smoke,INPUT);
  pinMode(LEDandBuzzer,OUTPUT);

  // Check if the AHT10/AHT20 sensor is connected and functioning
  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10); // Hangs the program in an infinite loop if sensor not found
  }

  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);

  LCD.init();
  LCD.backlight();

  //----------------------------------------------
   WiFi.begin(wifi_name, password);
    while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to Wifi!");
  }

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //-----------------------------------------------
}

void loop() {

  Pressure = digitalRead(ForcePin);  // Read the analog value from the force sensor and map it to a pressure range of 0 to 4095

  // Read the status of the flame sensor (0 for flame detected, 1 for no flame detected)
  SmokeReadings = analogRead(Smoke); // getting sensor reading from A0 pin
  FlameDetected = digitalRead(FlameSensor);

  upload_bool(flame_path, !FlameDetected);
  upload_int(smoke_path, SmokeReadings);
  // Get fresh temperature and humidity data from the AHT10/AHT20 sensor
  aht.getEvent(&humidity, &temp);

  upload_int(temp_path, temp.temperature);

  IR_Readings = analogRead(IR_Pin);  // Read the analog value from the IR sensor
  int distanceCM = map(IR_Readings, minAnalogValue, maxAnalogValue, minDistanceCM, maxDistanceCM);
  upload_int(ir_path, distanceCM);

  Serial.print("Analog Value: ");
  Serial.print(IR_Readings);
  Serial.print(" | Distance: ");
  Serial.print(distanceCM);
  Serial.println(" cm");

  RainDropReadings = analogRead(RainSensor); // Read the analog value from the rain sensor

  LDR_Readings = analogRead(LDR);   
  Serial.println(LDR_Readings);
  upload_int(ldr_path, LDR_Readings);

  Serial.println("-------------------------------------");

  if(!Someone){
    Security_System();
    Someone = true;
  }
  Lock_System();
  Alarm_System();
  Fan_System();
  Door_System();
  RainDrop_System();
  Brightness_System();

  Serial.println("-------------------------------------\n");
}

// Function to handle locking the gate
void Lock_System(){
  if(Pressure == 1){
    Serial.println("Bye");
    upload_bool(gate_path, true);

    LCD.setCursor(6, 0);
    LCD.print("Bye");

    Servo_Gate.write(180);
    delay(1500);
    Servo_Gate.write(90);
    delay(5000);
    Servo_Gate.write(0);
    delay(1500);
    Servo_Gate.write(90);

    Someone = false;
  }else{
    upload_bool(gate_path, false);
  }     
}

// Function to handle alarm system
void Alarm_System(){
  if (SmokeReadings > SmokeThreshold) {
    upload_bool(alarm_path, true);
    Serial.println("Smoke detected !!");
    tone(LEDandBuzzer, Smoke_TONE);    // Turn ON the LED
    Emergency_System();
  } else {
    upload_bool(alarm_path, false);
    Serial.println("No Smoke...");
    noTone(LEDandBuzzer);     // Turn OFF the LED
  }

  // Check if a flame is detected
  if (FlameDetected == 0) {
    upload_bool(alarm_path, true);
    // If flame is detected, perform the following actions
    Serial.println("Flame detected...! Take action immediately.");
    tone(LEDandBuzzer, Flame_TONE);     // Turn ON the LED
    Emergency_System();
  } else {
    upload_bool(alarm_path, false);
    // If no flame is detected, perform the following actions
    Serial.println("No flame detected. Stay cool.");
    noTone(LEDandBuzzer);      // Turn OFF the LED
  }
}

// Function to control fan system
void Fan_System(){

  bool signal = read(fan_path);
    if(signal){
      digitalWrite(IN1, HIGH);     
      digitalWrite(IN2, LOW);  
    }else{
      digitalWrite(IN1, LOW);      
      digitalWrite(IN2, LOW);   
    }

}

// Function to handle door system
void Door_System(){
  if(IR_Readings < 3000){
    upload_bool(door_path, true);
    Serial.println("Person Detected");
    Servo_Door.write(0);
    delay(5000);
    Servo_Door.write(90);
  }
  else{
    upload_bool(door_path, false);
    Serial.println("No Person");
  }
}

// Function to handle raindrop system
void RainDrop_System(){
  if(RainDropReadings < 3000){
    upload_bool(rain_path, true);
    upload_bool(window_path, true);
    Serial.println("Raining");
    if(Rain == false){
      Servo_Window.write(0);
      delay(700);
      Servo_Window.write(90);
      Rain = true;
    }
  }
  else{
    upload_bool(rain_path, false);
    upload_bool(window_path, false);
    Serial.println("No Rain");
    if(Rain == true){
      Servo_Window.write(180);
      delay(1000);
      Servo_Window.write(90);
      Rain = false;
    }
  }
}

// Function to read keypad input
void readKeypad() {
  key = keypad.waitForKey();
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("Enter Password:");
  
  if (key != '#') {
    String knov = String(key);
    pad += knov;
    Serial.println(pad);
    LCD.setCursor(0, 1);
    LCD.print(pad);
  }
  
  if (key == '*') {
    pad = "";
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("Enter Password:");
  }
}

// Function to manage security system with keypad
void Security_System(){
  pad = "";
    String password = "1234";
    LCD.clear();
    LCD.setCursor(0, 0);
    LCD.print("Enter Password:");
    while (pad != password) {
      pad = "";
      key = ' ';
      
      // Read keypad input until '#' is pressed
      while (key != '#') {
        readKeypad();
      }
      
      if (pad == password) {
        Serial.println("Welcome Home");
        LCD.clear();
        LCD.setCursor(2, 0);
        LCD.print("Welcome Home");
        
        Servo_Gate.write(180);
        delay(1500);
        Servo_Gate.write(90);
        delay(5000);
        Servo_Gate.write(0);
        delay(1500);
        Servo_Gate.write(90);
        LCD.clear();
      } else {
        LCD.setCursor(3, 0);
        Serial.println("Try Again");
        LCD.clear();
        LCD.setCursor(3, 0);
        LCD.print("Try Again");
        delay(1000);
      }
    }
}

// Function to control LED brightness
void Brightness_System(){

  bool signal = read(light_path);
  if(signal){
    tone(LEDandBuzzer, Brightness_TONE);
  }else{
    noTone(LEDandBuzzer);
  }

}

void Emergency_System(){

  Servo_Gate.write(180);
  upload_bool(door_path, false);
  upload_bool(gate_path, false);
  delay(1500);
  Servo_Gate.write(90);

  Servo_Door.write(0);

  delay(5000);

  Servo_Gate.write(0);
  delay(1500);
  Servo_Gate.write(90);

  Servo_Door.write(90);
  upload_bool(door_path, true);
  upload_bool(gate_path, true);

}