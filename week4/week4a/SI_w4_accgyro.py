import serial

try:
    ser = serial.Serial('COM5', 9600)
    while True:
        data = ser.readline().decode('utf-8').strip()
        print(data)
except serial.SerialException as e:
    print(f"An error occurred: {e}")
except KeyboardInterrupt:
    print("Script terminated by user.")
finally:
    ser.close()
