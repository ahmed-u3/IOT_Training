#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//---------------------------------------------------------
#include <FirebaseESP32.h>
#include <WiFiClientSecure.h>

#define FIREBASE_HOST "https://esp-32-2c84d-default-rtdb.firebaseio.com/z"   // RealTime Database Host Link
#define FIREBASE_AUTH "985113887683"                                                // RealTime Database Host Authorization Number

const char *wifi_name = "Mi 10T";
const char *password = "262004321";

String position1_path = "/UsersData/Garage/Position-1";
String position2_path = "/UsersData/Garage/Position-2";
String position3_path = "/UsersData/Garage/Position-3";
String position4_path = "/UsersData/Garage/Position-4";

String paths[] = {position1_path, position2_path, position3_path, position4_path};

FirebaseData firebase;
//---------------------------------------------------------

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int irPins[] = {33, 32, 35, 34};
const int ledPins[] = {19, 4, 15, 2};  // Pins for LEDs
const int buzzerPin = 14;
const int numSlots = sizeof(irPins) / sizeof(irPins[0]);

bool slotOccupied[numSlots] = {false};

void setup() {

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Garage Slots:");
  display.println();
  display.display();

  for (int i = 0; i < numSlots; i++) {
    pinMode(irPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }

  pinMode(buzzerPin, OUTPUT);

  // Blink LEDs and sound the buzzer two times at the beginning
  for (int blinkCount = 0; blinkCount < 2; blinkCount++) {
    for (int i = 0; i < numSlots; i++) {
      digitalWrite(ledPins[i], HIGH);  // Turn on LED
    }
    digitalWrite(buzzerPin, HIGH);  // Sound the buzzer
    delay(500);  // Blink for 500ms
    for (int i = 0; i < numSlots; i++) {
      digitalWrite(ledPins[i], LOW);  // Turn off LED
    }
    digitalWrite(buzzerPin, LOW);   // Turn off buzzer
    delay(500);  // Blink for 500ms
  }

  //------------------------------------------
  WiFi.begin(wifi_name, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting to Wifi!");
  }
  WiFi.mode(WIFI_STA);

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);    // Connecting to firebase
  //------------------------------------------


}

void upload(bool slotOccupied[]){

  for(int i = 0; i < 4; i++){
    if (Firebase.setBool(firebase, paths[i], slotOccupied[i])) {
      //Serial.println("Data sent to Firebase!");
    } else {
     // Serial.println("Failed to send data to Firebase.");
      Serial.println(firebase.errorReason());
    }
  }
}

void loop() {

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Garage Slots:");
  Serial.println("Garage Slots:");
  display.println();

  for (int i = 0; i < numSlots; i++) {
    int sensorValue = analogRead(irPins[i]);
    bool occupied = sensorValue < 3000;
    
    if (occupied && !slotOccupied[i]) {
      digitalWrite(ledPins[i], LOW);
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(buzzerPin, LOW);
      slotOccupied[i] = true;
      upload(slotOccupied);
    }
    else if (!occupied) {
      digitalWrite(ledPins[i], HIGH);
      slotOccupied[i] = false;
      upload(slotOccupied);
    }
    
    display.print("Slot ");
    display.print(i + 1);
    display.print(": ");
    display.println(occupied ? "Occupied" : "Empty");
    Serial.print("Slot ");
    Serial.print(i + 1);
    Serial.println(occupied ? " Occupied" : " Empty");
  }
  
  Serial.println("----------------------");
  display.display();
}