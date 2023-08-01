
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 display(128,32,&Wire,-1); //pixel,pixel,pass address to know sda and scl , reset pin (there's no reset pin so will make it = -1)

#define IRSensor A0 // connect ir sensor module to Arduino pin A0

void setup() 
{
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internall
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //0x3c is the display address
}

void loop() 
{
  int sensorStatus = analogRead(IRSensor);
  
  
  display.clearDisplay(); // Clear the display
  
  display.setTextSize(1); // set size of the Text (take from 1-8)
  display.setTextColor(WHITE);
  display.setCursor(0,0); // Bt7dd t7ot el text fein
  display.println("Distance from IR: ");

  display.setTextSize(2); // set size of the Text (take from 1-8)
  display.setTextColor(WHITE);//Ayo it can take colors Gamed
  display.setCursor(40,15); // Bt7dd t7ot el text fein
  display.println(sensorStatus);
  display.display(); //To display 
  delay(2000);

}
