#include <LiquidCrystal.h>
#include <Keypad.h>
#include <ESP32Servo.h>

#define d 50
#define servo_pin 18
#define ir_pin 35
#define ldr_pin 34
#define led 21
#define LINE Serial.println("--------------------------------------------")

const int rs = 22, en = 23, d4 = 5, d5 = 4, d6 = 2, d7 = 15;
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = { {'1','2','3','A'}, 
                          {'4','5','6','B'}, 
                          {'7','8','9','C'}, 
                          {'*','0','#','D'} };

byte rowPins[ROWS] = { 13 , 12 , 14 , 27 }; 
byte colPins[COLS] = { 26 , 25 , 33 , 32 }; 

char pass[6];
char correct_pass[]="A12B34";
int i = 0 , count = 0;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Servo servo;


void door(String cond , int degree , int stepdelay){
  lcd.clear();
  servo.write(degree);
  delay(d);
  Serial.print("Door is ");
  Serial.println(cond);
  lcd.setCursor(0,0);
  lcd.print("Door is ");
  lcd.print(cond);
  lcd.print("            ");
  delay(stepdelay);
}

void print_signal(String sensor_1 , String sensor_2,int stepdelay){
  Serial.print("ir : ");
  Serial.print(sensor_1);
  Serial.print(" cm , ldr : ");
  Serial.print(sensor_2);
  Serial.println(" lux");
  LINE;
  lcd.setCursor(0,1);
  lcd.print("ir:");
  lcd.print(sensor_1);
  lcd.print(",");
  lcd.print("ldr:");
  lcd.print(sensor_2);
  delay(stepdelay);
}

void check_pass(char pass[] , char corr_pass[]){
  for(int j = 0 ; j<6 ; j++){
      if( pass[j] == correct_pass[j] ) count++;
    }
  
  if( count == 6 ){
      String ir_signal = String(analogRead(ir_pin));
      String ldr_signal = String(analogRead(ldr_pin));

      Serial.println("Correct pass");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Correct Pass     ");
      delay(1000);
      digitalWrite(led,HIGH);
      door("Open",180,0);
      print_signal(ir_signal,ldr_signal,5000);
      i = 0;
      count = 0;
      door("Close",0,1000);
      LINE;
      Serial.print("Enter the Pass : ");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter the Pass");
      lcd.setCursor(0,1);
    }

    else{
      Serial.println("Wrong Pass");
      LINE;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Wrong Pass        ");
      delay(2000);
      i = 0;
      count = 0;
      Serial.print("Enter the pass : ");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter the pass       ");
      lcd.setCursor(0,1);
    }
}


void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  Serial.print("Enter the pass : ");
  lcd.clear();
  lcd.print("Enter the Pass");
  lcd.setCursor(0,1);
  pinMode(led,OUTPUT);
  pinMode(ir_pin,INPUT);
  pinMode(ldr_pin,INPUT);
  servo.attach(servo_pin);
  servo.write(0);
  delay(d);
}

void loop() {
  if(Serial.available()){

    String receivedString = Serial.readString();
    receivedString.toCharArray(pass,7);

    int str_len = strlen(pass);
    for(;str_len-1;str_len--){
      Serial.print("*");
      }
    Serial.println();
    check_pass(pass,correct_pass);
    strcpy(pass,"        ");
  }
  else{
    
    char key = keypad.getKey();
    if( key >= '0' && key != 'D' ){
      lcd.print("*");
      pass[i] = key;
      i++;
    }
    else if( key == 'D' ){
      int str_len = strlen(pass);
      for(;str_len;str_len--){
        Serial.print("*");
        }
      Serial.println();
      check_pass(pass,correct_pass);
      strcpy(pass,"\0");
    }
  }
}