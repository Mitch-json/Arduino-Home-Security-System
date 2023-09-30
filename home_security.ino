#include <LiquidCrystal.h> //Load the LCD library
#include <Keypad.h>   //use the Keypad libraries
LiquidCrystal lcd(13, 12, 11, 10, 9, 8, 7);

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = 
{
  { 
    '1','2','3','A'      }
  ,
  { 
    '4','5','6','B'      }
  ,
  { 
    '7','8','9','C'      }
  ,
  { 
    '*','0','#','D'      }
};

byte rowPins[ROWS] = {40, 39, 38, 37}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {36, 35, 34, 33}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

String pin[4] = {"1","2","3","4"};
String unlockPin[4];

const int delayTime = 2000;

const int remoteA = 22;
const int remoteB = 23;
const int systemOn = 2;
const int systemOff = 3;
const int doorSensor = 5;
const int buzzer = 50;
const int motionSensor3 = 45;
const int motionSensor2 = 46;
const int motionSensor = 47;              // the pin that the motion sensor is atteched to
const int laser0 = 48;
const int laser1 = 49;
const int laser3 = 51;
const int laser4 = 52;
const int laser5 = 53;

int motionVal = 0;                 // variable to store the motion sensor status (value)
int motionVal2 = 0;                 // variable to store the motion sensor status (value)
int motionVal3 = 0;                 // variable to store the motion sensor status (value)

int light0 = 0; // store the current light value
int light1 = 1000; // store the current light value
int light3 = 1000; // store the current light value
int state;
bool systemStatus = false;

String starPin = "";

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);          /* Initialize 16x2 LCD */
  lcd.clear();
  lcd.setCursor(0,0);        /* Set cursor to column 0 row 0 */
  lcd.print("system off");
  
  pinMode(remoteA,INPUT); 
  pinMode(remoteB, INPUT);
  pinMode(systemOn, OUTPUT);
  pinMode(doorSensor, INPUT_PULLUP);
  pinMode(motionSensor, INPUT);    // initialize motion sensor as an input
  pinMode(motionSensor2, INPUT);    // initialize motion sensor as an input
  pinMode(laser0, OUTPUT);
  pinMode(laser1, OUTPUT);
  pinMode(laser3, OUTPUT);
  
  Serial.begin(9600); 
}

void loop() {
  // put your main code here, to run repeatedly:
  int remoteValueA = digitalRead(remoteA);
  int remoteValueB = digitalRead(remoteB);

  if(remoteValueA == HIGH){
      bool alarmOn = turnAlarmStatus(true); 
  }
  if(remoteValueB == HIGH){
      bool alarmOff = turnAlarmStatus(false);
    
  }

  
  if(systemStatus == true){
    digitalWrite(systemOff, LOW);
    light1 = analogRead(A1); // read and save value from PR
    light0 = analogRead(A0); // read and save value from PR
    light3 = analogRead(A2); // read and save value from PR
  }else{
    digitalWrite(systemOff, HIGH);
  }
  
  
  motionVal = digitalRead(motionSensor);   // read motion sensor value
  motionVal2 = digitalRead(motionSensor2);   // read motion sensor value
  motionVal3 = digitalRead(motionSensor3);   // read motion sensor value

  if(light0 > 50){
    digitalWrite(laser0, LOW);
    if (motionVal == HIGH && systemStatus == true) {           // check if the sensor is HIGH
      while(motionVal==HIGH){
        tone(buzzer, 10);
        int off = digitalRead(remoteB);
        if(off == HIGH){
            bool alarmOff = turnAlarmStatus(false);
            if(alarmOff == true){
              noTone(buzzer);
              break;
            }
            
        }
      }
    } 
    else {
        noTone(buzzer);
    }
  }

  if(light1 < 500){
    digitalWrite(laser1, LOW);
    if (motionVal2 == HIGH && systemStatus == true) {           // check if the sensor is HIGH
      while(motionVal2==HIGH){
        tone(buzzer, 10);
        int off = digitalRead(remoteB);
        if(off == HIGH){
            bool alarmOff = turnAlarmStatus(false);
            if(alarmOff == true){
              noTone(buzzer);
              break;
            }
            
        }
      }
    } 
    else {
        noTone(buzzer);
    }
  }

  if(light3 < 500){
    digitalWrite(laser3, LOW);
    if (motionVal3 == HIGH && systemStatus == true) {           // check if the sensor is HIGH
      while(motionVal3==HIGH){
        tone(buzzer, 10);
        int off = digitalRead(remoteB);
        if(off == HIGH){
            bool alarmOff = turnAlarmStatus(false);
            if(alarmOff == true){
              noTone(buzzer);
              break;
            }
            
        }
      }
    } 
    else {
        noTone(buzzer);
    }
  }

  if(systemStatus == true){
    state = digitalRead(doorSensor);

    if(state == HIGH){
      tone(buzzer, 400);
    }
    else{
      noTone(buzzer);
    }
  }
  
}

bool turnAlarmStatus(bool sysStatus){
    lcd.clear();
    lcd.setCursor(0,0);        /* Set cursor to column 0 row 0 */
    lcd.print("Input PIN");
    lcd.setCursor(0,1);        /* Set cursor to column 0 row 0 */
    lcd.print("PIN:");
    starPin = "PIN:";
    for(int i=0; i<4; i++){
      char customKey = customKeypad.getKey();
      int res;
      while(!customKey){
        customKey = customKeypad.getKey();
      }
  
      
      lcd.setCursor(0,1);
      starPin.concat("*");
      lcd.print(starPin);
      unlockPin[i] = customKey;
      delay(500);
      if(i == 3){
        
        if(pin[0]==unlockPin[0] && pin[1]==unlockPin[1] && pin[2]==unlockPin[2] && pin[3]==unlockPin[3] ){
          if(sysStatus == true){
            systemStatus = true;
            digitalWrite(laser0, HIGH); 
            digitalWrite(laser1, HIGH);
            digitalWrite(laser3, HIGH);
            digitalWrite(systemOn, HIGH);
            
            lcd.clear();
            lcd.setCursor(0,0);        /* Set cursor to column 0 row 0 */
            lcd.print("system active");
          }else if(sysStatus == false){
            systemStatus = false;
            light1 = 1000;
            light0 = 0;
            digitalWrite(systemOn, LOW);  
            digitalWrite(laser0, LOW);
            digitalWrite(laser1, LOW);
            digitalWrite(laser3, LOW);
            
            lcd.clear();
            lcd.setCursor(0,0);        /* Set cursor to column 0 row 0 */
            lcd.print("system off");
            return true;
          }
          
        }else{
          lcd.clear();
          lcd.setCursor(0,0);        /* Set cursor to column 0 row 0 */
          lcd.print("wrong pin! retry");
          delay(1000);
          
          lcd.setCursor(0,1);        /* Set cursor to column 0 row 0 */
          lcd.print("PIN:");
          starPin = "PIN:";
          i=-1;
        }
      }  
    }
}
