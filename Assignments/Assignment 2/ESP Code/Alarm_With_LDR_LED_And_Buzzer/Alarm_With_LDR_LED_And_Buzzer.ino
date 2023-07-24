int LDR_Val = 0;     /*Variable to store photoresistor value*/
int sensor =34;      /*Analogue Input for photoresistor*/
int led= 2;         /*LED output Pin*/
int buzzer = 19;    /*Buzzer output Pin*/

void setup() {

    Serial.begin(9600);     /*Baud rate for serial communication*/
    pinMode(led, OUTPUT);   /*LED Pin set as output */
    pinMode(buzzer, OUTPUT);   /*LED Pin set as output */

}
void loop() {

    LDR_Val = analogRead(sensor);   /*Analog read LDR value*/
    Serial.print("LDR Output Value: ");
    Serial.println(LDR_Val);   /*Display LDR Output Val on serial monitor*/

    if(LDR_Val < 100) {       /*If light intensity is LOW*/
        Serial.println("ALARM : LOW intensity ");
        digitalWrite(led,HIGH); /*LED Remains ON*/
        digitalWrite(buzzer,HIGH);
    }
    
    else {
     /*Else if Light intensity is HIGH LED will Remain OFF*/
        Serial.println("HIGH Intensity ");
        digitalWrite(led,LOW);  /* LED Turn ON LDR value is more than 100*/
        digitalWrite(buzzer,LOW);
    }
    delay(1000);     /*Reads value after every 1 sec*/

}