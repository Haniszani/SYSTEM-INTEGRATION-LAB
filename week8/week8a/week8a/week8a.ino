#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <BluetoothSerial.h>
#include <ESP32_Servo.h> 

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
BluetoothSerial SerialBT;

unsigned long delayTime;

void setup() {
    Serial.begin(9600);
    while (!Serial); // time to get serial running

    SerialBT.begin("ESP32_BT_Classic"); // Bluetooth device name
    Serial.println(F("BME280 test"));

    unsigned status;

    // default settings
    status = bme.begin(0X76);
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x");
        Serial.println(bme.sensorID(), 16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1)
            delay(10);
    }

    Serial.println("-- Default Test --");
    delayTime = 1000;

    Serial.println();
}

void loop() {
    if (SerialBT.available()) {
        processBluetoothCommand();
    }

    printTemperatureOverBluetooth();
    delay(delayTime);
}

void printTemperatureOverBluetooth() {
    SerialBT.print("Temperature = ");
    SerialBT.print(bme.readTemperature());
    SerialBT.println(" °C");
}

void processBluetoothCommand() {
    String command = SerialBT.readStringUntil('\n');
    command.trim();

    if (command == "ON") {
        // Replace PIN_NUMBER with the actual pin number you want to turn on
        analogWrite(5, 255);
    }
    if (command == "OFF") {
        // Replace PIN_NUMBER with the actual pin number you want to turn on
        analogWrite(5, 0);
    }
}