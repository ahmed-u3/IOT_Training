#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <Keypad.h>
#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

// Initialize the LCD display with I2C address 0x27, 16 columns, and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Create a Servo object and set the pin for the servo motor
Servo myservo;
int servoPin = 15;
int pos = 0;

// Define the pin for the IR sensor
int irPin = 35;

// Define the keypad layout and pin connections
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte rowPins[ROWS] = { 13, 12, 14, 27 };
byte colPins[COLS] = { 26, 25, 33, 32 };
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Import helper functions for token generation and RTDB
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// Define WiFi credentials
#define WIFI_SSID "Network goes BRR"
#define WIFI_PASSWORD "Ah01020038841"

// Define the API key obtained from Firebase console
#define API_KEY "Your_API_Key"

// Function to get a password input from the keypad
String getPassword() {
  String passwd = "";
  int i = 0;
  lcd.setCursor(0,0);
  
  Serial.println("Password:");
  while (true) {
    char customKey = customKeypad.getKey();
    if (customKey) {
      if (customKey == '#') {
        lcd.clear();
        break;
      } else if (customKey == '*') {
        passwd = "";
      } else {
        passwd += customKey;
        lcd.setCursor(i,0);
        lcd.print(customKey);
        i++;
        Serial.print(customKey);
      }
    }
  }
  return passwd;
}

// Define the user's email and password for authentication
#define USER_EMAIL "ayousry42@gmail.com"
#define USER_PASSWORD getPassword()

// Define the Firebase Realtime Database (RTDB) URL
#define DATABASE_URL "https://your-firebase-project-id.firebaseio.com/"

// Create a FirebaseData object to hold data and manage Firebase interactions
FirebaseData fbdo;

// Create a FirebaseAuth object to store authentication data
FirebaseAuth auth;

// Create a FirebaseConfig object to store configuration data
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;

void setup() {
  Serial.begin(115200);

  // Attach the servo to the specified pin
  myservo.attach(servoPin);

  // Initialize and configure the LCD display
  lcd.init();
  lcd.clear();
  lcd.backlight();

  // Connect to WiFi using the defined credentials
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Print the Firebase Client version
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  // Set the API key in the Firebase configuration
  config.api_key = API_KEY;

  // Set the user's email and password in the authentication object
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Set the RTDB URL in the configuration
  config.database_url = DATABASE_URL;

  // Reconnect WiFi and set response size for FirebaseData
  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(115200);

  // Define the base path for user data
  String base_path = "/UsersData/";

  // Set the token status callback for token generation task
  config.token_status_callback = tokenStatusCallback;

  // Initialize the Firebase library with authentication and configuration
  Firebase.begin(&config, &auth);
}

void loop() {
  // Update data every 1000 milliseconds if Firebase is ready
  if (millis() - dataMillis > 1000 && Firebase.ready()) {
    dataMillis = millis();

    // Read the analog value from the IR sensor
    int irValue = analogRead(irPin);

    // Create a base path for user data
    String base = "/UsersData/";

    // Create a path for IR data based on user ID
    String irPath = base + auth.token.uid.c_str();
    irPath += "/IR";

    // Set the IR value in the Firebase RTDB
    Serial.printf("Set IR... %s\n", Firebase.RTDB.setInt(&fbdo, irPath, irValue) ? "ok" : fbdo.errorReason().c_str());

    // Get and print the IR value from the Firebase RTDB
    Serial.printf("Get IR ... %s\n", Firebase.RTDB.getInt(&fbdo, irPath) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());

    // Map the IR value to a servo angle and update the servo position
    int pos = map(irValue,0,4095,0,180);
    myservo.write(pos);

    // Update the LCD display with IR and servo data
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("IR");
    lcd.setCursor(6,1);
    lcd.print(irValue);
    lcd.setCursor(0,0);
    lcd.print("Angle");
    lcd.setCursor(6,0);
    lcd.print(pos);

    // Delay for a second before the next update
    delay(1000);
  }
}
