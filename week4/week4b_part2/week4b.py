import serial
import json

with open("people.json", "r") as f:
    VIP = json.load(f)

with serial.Serial('COM9', 9600) as ser:
    print("RFID Data ready!")

    try:
        while True:
            data = ser.readline().decode()
            if data.startswith("Card UID:"):
                card = data.split(":")[1].strip()
                if card in VIP:
                    print(VIP[card]["Name"])
                    ser.write("A".encode())
                else:
                    print("Hello?")
                    ser.write("B".encode())

    except KeyboardInterrupt:
        ser.close()
        print("Serial Close")