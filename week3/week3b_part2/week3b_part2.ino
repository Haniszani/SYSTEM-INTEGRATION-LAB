#include <Servo.h>

Servo servo;
const int potentiometerPin = A0;  // Analog pin for the potentiometer
int potentiometerValue = 0;
int servoAngle = 0;  // Initial servo angle
char userInput = ' ';  // Variable to store user input

void setup() {
  servo.attach(9);     // Attach the servo to pin 9
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  // Read potentiometer value
  potentiometerValue = analogRead(potentiometerPin);

  // Map potentiometer value to servo angle (0-180 degrees)
  servoAngle = map(potentiometerValue, 0, 1023, 0, 180);

  // Control the servo
  servo.write(servoAngle);

  // Check for user input
  if (Serial.available() > 0) {
    userInput = Serial.read();
    
    if ((userInput >= 'A' && userInput <= 'Z') || (userInput >= 'a' && userInput <= 'z')) {
      // Stop the servo when a letter is received
      servo.write(0);  // Set the servo to the center position (90 degrees)
    }
  }
}