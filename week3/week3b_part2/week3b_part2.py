import serial
import time

ser = serial.Serial('COM9', 9600)
try:
 while True:
    angle = input("Enter servo angle (0-180 degrees): ")
    if angle.lower() == 'q':
        break
    angle = int(angle)
    if 0 <= angle <= 180:
        # Send the servo’s angle to the Arduino
        ser.write(str(angle).encode()) 
    else:
        print("Angle must be between 0 and 180 degrees.")
except KeyboardInterrupt:
 pass # Handle keyboard interrupt
finally:
 ser.close() # Close the serial connection
print("Serial connection closed.")