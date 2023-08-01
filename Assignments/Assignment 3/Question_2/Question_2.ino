#include <Keypad.h>
#include <ESP32Servo.h>

#define d 50
#define LINE Serial.println("--------------------------------------------")
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = { {'1','2','3','A'}, 
                          {'4','5','6','B'}, 
                          {'7','8','9','C'}, 
                          {'*','0','#','D'} };

byte rowPins[ROWS] = { 13 , 12 , 14 , 27 }; 
byte colPins[COLS] = { 26 , 25 , 33 , 32 }; 

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Servo servomotor;

int i = 0, pos = 0;
int startAngle = 0 , endAngle = 0 ;
bool stopRotation = false ;

void rotate(int stepDelay){

  endAngle = constrain(endAngle,0,180);

  int direction = (startAngle < endAngle) ? 1 : -1;

  loop_start:
  for(;startAngle != endAngle;startAngle+=direction){
    servomotor.write(startAngle);
    delay(stepDelay);

    checkStopRotation();

    if(stopRotation){
      break;
    }
  }
  endAngle = startAngle;
  stopRotation = false;
}

void checkStopRotation() {
  char key = keypad.getKey();
  if (key == '*') {
    Serial.println("STOP!!!");
    Serial.print("start angle -> ");
    Serial.println(startAngle);
    Serial.print("end angle -> ");
    Serial.println(endAngle);
    LINE;
    stopRotation = true;
  }
}

void setup() {
  Serial.begin(9600);
  //Serial.println("The program begin");
  servomotor.attach(5);
  servomotor.write(0);
  delay(50);
  Serial.println("U can use Keypad or Serial monitor");
}

void loop() {

  if (Serial.available()) {
    endAngle = Serial.parseInt(); 
    if (Serial.read() == '\n') {
      Serial.print("Received value: ");
      Serial.println(endAngle);
      LINE;
    }
  }
  else{
    char key = keypad.getKey();
    int value = key - 48;
    if ( value>=0 && value <=9 && key != 'A' && i<=3) {
      pos = (pos * 10) + value;
      i++;
    }
    else if(key == '#'){
      endAngle = 0;
      Serial.println("REST!!!");
      Serial.print("start angle -> ");
      Serial.println(startAngle);
      Serial.print("end angle -> ");
      Serial.println(endAngle);
      LINE;
    }
    else if (key == 'A' || i==3){
      endAngle = pos;
      Serial.print("start angle -> ");
      Serial.println(startAngle);
      Serial.print("end angle -> ");
      Serial.println(endAngle);
      LINE;
      pos = 0 ,i = 0;
    }
    else if(key == 'B' || key == 'C' || key == 'D'){
      Serial.println("unknown keyword");
    }
  }
  rotate(d);
}