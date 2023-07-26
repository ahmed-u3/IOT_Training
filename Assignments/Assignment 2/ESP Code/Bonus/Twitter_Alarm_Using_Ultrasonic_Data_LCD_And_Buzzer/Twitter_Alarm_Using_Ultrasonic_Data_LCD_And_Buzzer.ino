#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "time.h"
#include <TweESP32.h>          // Install from Github - https://github.com/witnessmenow/TweESP32
#include <TwitterServerCert.h> // included with above
#include <UrlEncode.h> //Install from library manager
#include <ArduinoJson.h> //Install from library manager
#include <LiquidCrystal.h> // Include the LiquidCrystal library

char ssid[] = "salsola";         // your network SSID (name)
char password[] = "bozo2010"; // your network key

const char *consumerKey = "hIrRkesBVTtAGCArZyhGWIFgK";
const char *consumerSecret = "a4dlqJ6Vg1y9aIExVsPOsgrNGoUTZgqQNM3BYOgon263YdOtKX";
const char *accessToken = "3366002295-VKPYV0ZfGcQt6btn1t6NLMBiWPuFXHIreWXdaqV";
const char *accessTokenSecret = "t3VbsjocqIlOVbOV0IFghiPsj2WGcPZXXgPMSBLFF92ec";
const char *ntpServer = "pool.ntp.org";

// For HTTPS requests
WiFiClientSecure client;
TweESP32 twitter(client, consumerKey, consumerSecret, accessToken, accessTokenSecret);

// Initialize the LCD module
const int rs = 22, en = 23, d4 = 5, d5 = 18, d6 = 19, d7 = 21;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define the pins for the ultrasonic sensor, LED, and buzzer
const int trigPin = 32;      // Digital output pin to trigger the ultrasonic sensor
const int echoPin = 33;      // Digital input pin to receive the echo from the ultrasonic sensor
const int ledPin = 2;        // Digital output pin for the LED
const int buzzerPin = 4;    // Digital output pin for the buzzer

// Define the proximity threshold value (adjust as needed)
const int proximityThreshold = 10; // Threshold distance in centimeters
bool flag = false;

void setup() {
  // put your setup code here, to run once:
  // Prompt the user to enter their name
  Serial.begin(9600);

  // Set up the number of columns and rows for the LCD
  lcd.begin(16, 2);
  
  // Clear the LCD screen
  lcd.clear();

  // Set the trigPin as an output and the echoPin as an input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set the LED and buzzer pins as outputs
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);  

  // Connect to the WiFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Required for Oauth for sending tweets
  twitter.timeConfig();

  // Checking the cert is the best way on an ESP32
  // This will verify the server is trusted.
  client.setCACert(twitter_server_cert);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Send a pulse to the trigPin to trigger the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the pulse from the echoPin
  long duration = pulseIn(echoPin, HIGH);

  // Calculate the distance in centimeters using the speed of sound (343 m/s) and the time it takes for the echo
  int distance = duration * 0.0343 / 2;

  // Check if the distance is below the threshold
  if (distance < proximityThreshold) {
    // Object detected!
    Serial.print("Object Detected !! : ");
    
    // Turn on the LED and buzzer
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);

    // Clear the LCD screen and set the cursor to the beginning of the first line
    lcd.clear();
    lcd.setCursor(0, 0);

    // Print a message on the LCD screen to indicate emergency
    lcd.print("Emergency !!! ");
    
    if(flag==false){
      twitter.sendTweet("IOT Assignment 7/24/2023");
      flag = true;
    } 

  } else {
    // No object nearby
    Serial.print("No Nearby Object : ");
    
    // Turn off the LED and buzzer
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);

    // Clear the LCD screen and set the cursor to the beginning of the first line
    lcd.clear();
    lcd.setCursor(0, 0);

    lcd.print("No Nearby Object : ");

    flag = false;
  }

  // Print the measured distance to the serial monitor
  Serial.print(distance);
  Serial.println(" cm");

  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm");

  // Add a small delay to avoid rapid triggering
  delay(100);
}
