import serial
import re
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Set your serial port name (you may need to change it based on your system)
serial_port = "COM7"  # Change this to the appropriate serial port

# Regular expression pattern to extract RGB values from the received string
pattern = re.compile(r'R:(\d+) G:(\d+) B:(\d+)')

# Initialize serial port
ser = serial.Serial(serial_port, baudrate=9600, timeout=1)

# Initialize plot
fig, ax = plt.subplots()
colors = ['red', 'green', 'blue']
bars = ax.bar(colors, [0, 0, 0], color=colors)

ax.set_ylim(0, 255)
ax.set_ylabel('RGB Values')
ax.set_title('RGB Values from Serial')

# Function to update plot data
def update(frame):
    # Send the AT+COLOR command every 1 seconds
    if frame % 20 == 0:
        ser.write(b'AT+COLOR\r\n')
    
    # Check if there is data available to read
    if ser.in_waiting > 0:
        # Read and decode the received data
        data = ser.readline().decode('utf-8').strip()
        print(data)  # Print received data for debugging
        
        # Extract RGB values using regular expression
        match = pattern.search(data)
        if match:
            rgb_values = [int(match.group(i)) for i in range(1, 4)]
            
            # Update bar chart data and color
            for bar, color, value in zip(bars, colors, rgb_values):
                bar.set_height(value)
                bar.set_color(color)

            # Redraw the bar chart
            fig.canvas.draw()

# Create an animation
ani = animation.FuncAnimation(fig, update, frames=100, blit=False)

# Show the plot
plt.show()

# Close the serial port when done
ser.close()
