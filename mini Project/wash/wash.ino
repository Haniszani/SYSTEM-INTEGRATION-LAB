#include <Stepper.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

const int stepsPerRevolution = 2048;
Stepper stepper(stepsPerRevolution, 10, 11, 12, 13);

Servo myservo;
int pos = 0;

LiquidCrystal_I2C lcd(0x3F, 16, 2); // I2C address 0x27, 16 column and 2 rows

const int limitSwitchPin = 2; 
const int RELAY_PINL = 5;
const int RELAY_PINK = 6;
int fix=15;

//ultrasonic sensor
const int ultrasonics = A0;
unsigned int ADCValue;



void setup() {
  Serial.begin(115200);
  myservo.attach(7);

  lcd.begin(16, 2);
  lcd.print("Washing Machine");

  pinMode(limitSwitchPin, INPUT_PULLUP);
  pinMode(RELAY_PINL, OUTPUT);
  pinMode(RELAY_PINK, OUTPUT);

}


void loop() {
    Serial.println(fix);
    ADCValue = analogRead(ultrasonics);
    // ADCValue *=3;
    //Serial.print("Present Length is: ");
    //Serial.print(ADCValue, DEC);
    //Serial.println("mm");


  //ultrasonic
  if(ADCValue < 80){
       digitalWrite(RELAY_PINL, HIGH);
       digitalWrite(RELAY_PINK, HIGH);
  }else{
       digitalWrite(RELAY_PINL, LOW);
       digitalWrite(RELAY_PINK, LOW);
  }
  
  int limitSwitchState = digitalRead(limitSwitchPin);
  //Serial.println(limitSwitchState);
  
  if (limitSwitchState == LOW) {
  lcdPrint("Door closed :)", "ready soon!");
  int washDuration = (1000);
  int rinseDuration = (1000);
  int spinDuration =(1000);

  wash(washDuration);
  rinse(rinseDuration);
  spin(spinDuration);

  lcdPrint("Process", "Complete!");
  complete();
  } else{
    lcdPrint("door open", "-_-");
    }

}

void wash(int duration) {
  lcdPrint("Washing", "1 sec");
  lcdPrint("Washing","In progress");
  waterpipe();
  lcdPrint("Washing", "In Progress");
  
  stepper.setSpeed(10);
  for (int i = 0; i < duration; i++) {
    stepper.step(1);
    delay(10);
  }

  lcdPrint("Washing", "Complete");
}

void rinse(int duration) {
  lcdPrint("Rinsing", "1 sec");
  lcdPrint("Rinsing","In progress");
  waterpipe();
  lcdPrint("Rinsing", "In Progress");
  
  stepper.setSpeed(10);
  for (int i = 0; i < duration; i++) {
    stepper.step(1);
    delay(10);
  }

  lcdPrint("Rinsing", "Complete");
}

void spin(int duration) {
  lcdPrint("Spinning", "1 sec");
  lcdPrint("Spinning", "In progress");
  lcdPrint("Spinning", "In Progress");

  stepper.setSpeed(50);
  for (int i = 0; i < duration; i++) {
    stepper.step(1);
    delay(5);
  }

  lcdPrint("Spinning", "Complete");
}

void complete() {
  fix++;
  lcdPrint("clothes", "Complete");
  //Serial.print(fix);

}


void lcdPrint(String line1, String line2) {
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void waterpipe() {
  lcdPrint("water valve", "open");
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  lcdPrint("water valve", "close");
  for (pos = 180; pos >= 0; pos -= 1) {
  myservo.write(pos);
  delay(15);}
}