#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "time.h"
#include <LiquidCrystal.h>
#include <ESP32Servo.h>


// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Tedata287"
#define WIFI_PASSWORD "TedataSalehM@720"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAJScpiHsICNlVdVHE3HoRKHl8uBti0Iw8"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "test99@gmail.com"
//#define USER_PASSWORD ""

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://login-a4430-default-rtdb.firebaseio.com/"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
int irsensor;
int pos=0;
String IR_path = "/irsensor";
String Distance_path = "/Distance";

String timePath = "/timestamp";

// // Ds18B20 Temperature sensor
// // GPIO where the DS18B20 is connected to
// const int oneWireBus = 4;
// // Setup a oneWire instance to communicate with any OneWire devices
// OneWire oneWire(oneWireBus);

// // Pass our oneWire reference to Dallas Temperature sensor
// DallasTemperature sensors(&oneWire);

// Parent Node (to be updated in every loop)
String parentPath;

int timestamp;
FirebaseJson json;

// Network time Protocl Server
const char* ntpServer = "pool.ntp.org";

int userID;
int IR_PIN = 35;
int LED_PIN = 27;
int BUZZER_PIN = 21;
int servo_pin=22;
Servo myservo; // create servo object to control a servo

// Timer variables (send new readings every thirt seconds)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 5000;

#include <Keypad.h>

#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // three columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM] = {13,12,32,33}; // GPIO18, GPIO5, GPIO17, GPIO16 connect to the row pins
byte pin_column[COLUMN_NUM] = {15,2,4,5};  // GPIO4, GPIO0, GPIO2 connect to the column pins

Keypad customKeypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

const int rs = 19, en = 23, d4 = 18, d5 = 14, d6 = 26, d7 = 25;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String passwordstr="";

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.print("\nConnected Successfully\nIp is: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    // Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

String getPass(){
  String Pass="";
  bool condition=false;
  lcd.clear();
  lcd.print("Enter Password: ");
  delay(500);
  while(!condition){
    char key=customKeypad.getKey();
    if(key){
      if(key!='#'){
        Pass+=key;
        lcd.print('*');
        Serial.print(key);
      }else if(key=='*'){
        Pass="";
        Serial.println("");
      }
      else{
        condition=true;
        lcd.clear();
        return Pass;
      }
    }
  }
}

void startFirebase(){
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION); // Print the Version of Firebase

  config.api_key = API_KEY; // Set the API Key of the Project

  String USER_PASSWORD = getPass(); // Gets the user password from the Keypad

  /* Authentication */
  auth.user.email = USER_EMAIL; // Set the User Email
  auth.user.password = USER_PASSWORD; // Set the User Password

  /* Configuration */
  Firebase.reconnectWiFi(true); // If WIFI is disconnected at any time, reconnect.
  fbdo.setResponseSize(4096); // Sets the size (in bytes) of the transferred data (between ESP and Firebase)

  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // To manage Firebase Authentication Tokens

  Firebase.begin(&config, &auth);
  delay(5000);

  if(Firebase.ready()){ // If Firebase Started
    Serial.println("Firebase Authenticated Successfully");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print( "Welcome to your" );
    lcd.setCursor(0, 1);
    lcd.print("home account");
    delay(2000);
    String userID = auth.token.uid.c_str(); // Get the user's unique ID (UID)
  }
  else{
    Serial.println("Firebase Could Not Authenticate");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please enter");
    lcd.setCursor(0, 1);
    lcd.print("another password");
    delay(3000);
    startFirebase();
}
}

void setup() {
  Serial.begin(115200);
  //start Wifi
  initWiFi();

  //Configure times
  configTime(0, 0, ntpServer);

  startFirebase();

  databasePath = "/UsersData/" + uid + "/readings";
  myservo.attach(servo_pin); // attaches the servo on pin 18 to the servo object
  pinMode(IR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  lcd.begin(16, 2);
  lcd.print("System Starting Up...");
  delay(900);
}

void Servo_rotate(int angle){
  for (pos; pos <= angle; pos += 1) { // goes from 0 degrees to 80 degrees
  // in steps of 1 degree
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15); // waits 15ms for the servo to reach the position
  }
  for (pos; pos >= 0; pos -= 1) { // goes from 80 degrees to 0 degrees
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15); // waits 15ms for the servo to reach the position
  }
}

void Critical(int distance){
if(distance<3){
  lcd.clear();
  lcd.print("Critical distance detected, Waiting Admin" );
  digitalWrite(BUZZER_PIN,HIGH);
  digitalWrite(LED_PIN,HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN,LOW);
  digitalWrite(LED_PIN,LOW);
  Firebase.RTDB.getInt(&fbdo,"/condition");
  double condition = fbdo.to<double>();
  if(condition==1){
    Firebase.RTDB.getInt(&fbdo,"/Servo");
    int angle = fbdo.to<int>();
    lcd.clear();
    lcd.print("WELCOME");
    Servo_rotate(angle);
  }else{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Please visit us another time");
    lcd.setCursor(0, 1);
    lcd.print("another time");

  }
}
}
double get_Distance;
void loop() {
  char key = customKeypad.getKey();
    if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)) {
      Critical(analogRead(IR_PIN));
      sendDataPrevMillis = millis();
      //Get current timestamp
      timestamp = getTime();
      Serial.print("time: ");
      Serial.println(timestamp);
      parentPath = databasePath + "/" + String(timestamp);
      irsensor = analogRead(IR_PIN);
      json.set(IR_path.c_str(),irsensor);
      double distance = map(irsensor, 0, 4095, 0.0, 5.0);
      delay(400);
      json.set(Distance_path.c_str(), distance);
      json.set(timePath, String(timestamp));
      Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
      delay(2000);
      Serial.printf("Get int... %s\n", Firebase.RTDB.getInt(&fbdo, String(parentPath + "/irsensor")) ? "got new IR Value" : fbdo.errorReason().c_str());
      int get_IR = fbdo.to<int>();
      Serial.printf("Get int... %s\n", Firebase.RTDB.getInt(&fbdo, String(parentPath + "/Distance")) ? "got new Distance" : fbdo.errorReason().c_str());
      get_Distance = fbdo.to<int>();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("IR: ");
      lcd.print(get_IR);
      lcd.setCursor(0, 1);
      lcd.print("Distance: ");
      lcd.print(get_Distance);
      Serial.println(get_Distance);
      Serial.println("-------------------------------------");
      delay(300);
  }   Critical(analogRead(IR_PIN));

}