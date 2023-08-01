// include the library code:
#include <LiquidCrystal.h>
#include <IRremote.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

#define IRSensor A0  // connect ir sensor module to Arduino pin A0
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
IRrecv irReceiver(IRSensor);
decode_results irResults;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  Serial.begin(9600);

  irReceiver.enableIRIn();

}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  int sensorStatus = analogRead(IRSensor);
  // print the number of seconds since reset:
  if (sensorStatus!=0){
  lcd.clear();
  lcd.print("Distance: ");
  lcd.setCursor(0, 1);
  lcd.print(sensorStatus);
  Serial.println(sensorStatus);
  irReceiver.resume();

  }else {
    lcd.clear();
    lcd.print("IR sensor removed");
    Serial.println("IR sensor removed");
  }
  delay(1000);
}