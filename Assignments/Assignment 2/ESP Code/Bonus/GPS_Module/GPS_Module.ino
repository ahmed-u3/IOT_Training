#include <HardwareSerial.h>
#include <TinyGPS++.h>

HardwareSerial gpsSerial(2); // use UART2

// Create a TinyGPS++ object
TinyGPSPlus gps;

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(9600);
}

void loop()
{
  // Displays information when new sentence is available.
  while (gpsSerial.available() > 0)
    Serial.write(gpsSerial.read());
}