#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h> //vscode
#include <Servo.h>

const int green = 2;
const int red = 3;

Servo servo;
char input;

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
servo.attach(5);
servo.write(90);
Serial.begin(9600);		// Initialize serial communications with the PC
while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
SPI.begin();			// Init SPI bus
mfrc522.PCD_Init();		// Init MFRC522
delay(4);				// Optional delay. Some board do need more time after init to be ready, see Readme
mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

}

void loop() {
	// Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}

	// Dump debug info about the card; PICC_HaltA() is automatically called
	mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

	if(Serial.available() > 0)
	{
		input = Serial.read();
		if(input == 'A')
		{
			digitalWrite(red,LOW);
			servo.write(180); // turn to 180;
			digitalWrite(green,HIGH);
			delay(2000);
			servo.write(90); // turn to 90;
			digitalWrite(green,LOW);
			digitalWrite(red,HIGH);
		}
		else
		{
			servo.write(90); // Do nothinng, go back to default
			digitalWrite(red,HIGH);
			digitalWrite(green,LOW);
		}
	}
}