import serial
import time

# Define the serial port
ser = serial.Serial('COM7', 9600)  # Replace 'COMx' with your actual COM port

def read_color():
    # Send a command to request color data
    ser.write(b'ReadColor\n')

    # Read the response from the sensor
    response = ser.readline().decode('utf-8').strip()

    # Parse the response (adjust based on your sensor's output format)
    color_data = response.split(',')
    red, green, blue = map(int, color_data)

    return red, green, blue

# Example usage
while True:
    try:
        red, green, blue = read_color()
        print(f'Red: {red}, Green: {green}, Blue: {blue}')
        time.sleep(1)  # Adjust the delay based on your needs
    except KeyboardInterrupt:
        break

# Close the serial port when done
ser.close()
