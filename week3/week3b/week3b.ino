#include <Servo.h>

Servo servo;
int servoPin = 9; //initiate pin on arduino 
int angle = 90; //inital angle

void setup() {
  servo.attach(servoPin); 
  servo.write(angle);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    angle = Serial.parseInt(); //parseInt reads characters from the serial input until it encounters a non numeric charcter, then it converts the collected character into intigers
    servo.write(angle);
    delay(15);  // Add a small delay for smoother motion
  }
}

/*
note:the arduino sketch reads angle commands from the serial port and updates the servo position accordingly
note:the python then establishes a serial connection with the arduino using pyserial library 
note:the python script allows user to input and angle between 0 and 180 degrees. it send the signal to the arduino via the serial port 
note: the arduino process the recieved angle and moves the servo motor accordingly */