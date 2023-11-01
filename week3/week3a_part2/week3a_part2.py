from time import sleep 

import serial 

  

serialport="COM5" 

baudrate=9600 

dt=0.05 

  

#open port 

ser = serial.Serial(serialport, baudrate ) 

if ser.is_open : 

    print(f"Serial port {ser.name} is open.") 

    data =[] 

    N=40 

  

    for k in range(N): 

        b = ser.readline() 

        strn=b.decode() 

        strl =strn.rstrip() 

        flt = float(strl) 

        print("Potentiometer Value:", flt) 

        data.append(flt) 

        sleep(dt) 

  

#close port 

ser.close() 

if not ser.is_open : 

    print(f"Serial port {ser.name} is closed") 

  

#plot reading 

import matplotlib.pyplot as plt 

import numpy as np 

  

#x = (0:(N-1))*dt 

x = np.arange (0, N*dt, dt) 

plt.plot( x, data) 

plt.xlabel('Time (second)') 

plt.ylabel('Reading (0-1023)') 

plt.title('Potentiometer Reading vs. Time' ) 

plt.show() 

  

exit() 