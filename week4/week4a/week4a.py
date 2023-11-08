import serial
import matplotlib.pyplot as plt

ser = serial.Serial('COM4', 9600)

# Create empty lists to store sensor data
accel_x = []
accel_y = []

# Create a live graph
plt.ion()
fig, ax = plt.subplots()
ax.set_xlim(-20000, 20000)
ax.set_ylim(-20000, 20000)
line, = ax.plot([], [], 'b-')
plt.title("Accelerometer Data")

while True:
    data = ser.readline().decode('utf-8').strip()
    if data.startswith("Detected Gesture: "):
        gesture = data.split(": ")[1]
        if gesture == "Gesture 1":
            # Perform an action for Gesture 1
            print("Action for Gesture 1")
        elif gesture == "Gesture 2":
            # Perform an action for Gesture 2
            print("Action for Gesture 2")
        # Add more gesture actions as needed
    else:
        # Parse sensor data
        try:
            ax, ay = [int(val) for val in data.split(',')]
            accel_x.append(ax)
            accel_y.append(ay)

            # Update the graph
            line.set_data(accel_x, accel_y)
            plt.pause(0.01)  # Adjust the pause value as needed for real-time updates

        except ValueError:
            pass

plt.ioff()
plt.show()
