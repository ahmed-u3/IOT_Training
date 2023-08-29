#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
int irPin = 35;
/* 1. Define the WiFi credentials */
#define WIFI_SSID "Network goes BRR"
#define WIFI_PASSWORD "Ah01020038841"

/** 2. Define the API key
 *
 * The API key (required) can be obtained since you created the project and set up
 * the Authentication in Firebase console. Then you will get the API key from
 * Firebase project Web API key in Project settings, on General tab should show the
 * Web API Key.
 *
 * You may need to enable the Identity provider at https://console.cloud.google.com/customer-identity/providers
 * Select your project, click at ENABLE IDENTITY PLATFORM button.
 * The API key also available by click at the link APPLICATION SETUP DETAILS.
 *
 */
#define API_KEY "AIzaSyDnTrfYM4_iSAo20kcp80IBikoqxDcNPzo"

/* 3. Define the user Email and password that already registerd or added in your project */
#define USER_EMAIL "ayousry42@gmail.com"
#define USER_PASSWORD "123456"

/* 4. If work with RTDB, define the RTDB URL */
#define DATABASE_URL "https://esp-32-2c84d-default-rtdb.firebaseio.com/"

/* 6. Define the Firebase Data object */
FirebaseData fbdo;

/* 7. Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* 8. Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;

void setup() {
  

  Serial.begin(115200);

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

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL */
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  String base_path = "/UsersData/";

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h

  /* Initialize the library with the Firebase authen and config */
  Firebase.begin(&config, &auth);
}

void loop() {
  if (millis() - dataMillis > 5000 && Firebase.ready()) {
    dataMillis = millis();
    String base = "/UsersData/";
    String intPath = base + auth.token.uid.c_str(); // user uid of current user that sign in with Emal/Password
    intPath += "/test/int";
    String irPath = base + auth.token.uid.c_str();
    irPath += "/test/IR";
    Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, intPath, count++) ? "ok" : fbdo.errorReason().c_str());

    // Getting IR value 
    Serial.printf("Get IR ... %s\n",Firebase.RTDB.getInt(&fbdo,irPath) ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());

    delay(1000);
    Firebase.RTDB.deleteNode(&fbdo, intPath);
  }
}
