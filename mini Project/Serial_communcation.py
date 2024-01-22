import serial
import threading
import tkinter as tk
import requests

stop = False
espdata = ""
gamedata = ""
washdata = ""
baud_rate = 115200


def user():
    global stop, espdata
    esp32_serial_port = "COM12"
    esp32_ser = serial.Serial(esp32_serial_port, baud_rate, timeout=1)
    while not stop:
        got_or_not = esp32_ser.readline().decode('utf-8').strip()
        if got_or_not != "":
            espdata = got_or_not
        data = gamedata
        esp32_ser.write(data.encode())
    esp32_ser.close()


def game():
    global stop, gamedata, espdata
    game_serial_port = "COM9"
    game_ser = serial.Serial(game_serial_port, baud_rate, timeout=1)
    while not stop:
        got_or_not = game_ser.readline().decode('utf-8').strip()
        if got_or_not != "":
            gamedata = got_or_not
        game_ser.write(espdata.encode())
    game_ser.close()


def wash():
    global stop, washdata, espdata
    wash_serial_port = "COM17"
    wash_ser = serial.Serial(wash_serial_port, baud_rate, timeout=1)
    while not stop:
        got_or_not = wash_ser.readline().decode('utf-8').strip()
        if got_or_not != "":
            washdata = got_or_not
        data = espdata + '\n'
        wash_ser.write('S'.encode())
    wash_ser.close()

def fixed():
    global washdata
    fix=4
    while not stop:
        if washdata != fix:
            fix=washdata
            requests.get("https://api.thingspeak.com/update?api_key=JWT4DD2CA5ETIWDL&field1= "+fix)



def update_data_label():
    data_label.config(
        text=f'ESP32: {espdata}\nGame: {gamedata}\nWash: {washdata}')
    root.after(100, update_data_label)


def on_closing(event=None):
    global stop
    stop = True
    root.destroy()


def start_reading():
    global stop
    stop = False
    espthread = threading.Thread(target=user)
    espthread.start()
    gamethread = threading.Thread(target=game)
    gamethread.start()
    washthread = threading.Thread(target=wash)
    washthread.start()
    fixedthread= threading.Thread(target=fixed)
    fixedthread.start()
    update_data_label()


root = tk.Tk()
root.title("Serial Data Monitor")
root.geometry("800x200")

frame_esp = tk.Frame(root, bg="#FFD700", padx=10, pady=10)
frame_esp.grid(row=0, column=0, padx=10, pady=10)

frame_game = tk.Frame(root, bg="#90EE90", padx=10, pady=10)
frame_game.grid(row=0, column=1, padx=10, pady=10)

frame_wash = tk.Frame(root, bg="#FF6347", padx=10, pady=10)
frame_wash.grid(row=0, column=2, padx=10, pady=10)

data_label = tk.Label(root, text="", font=("Helvetica", 12), justify="left")
data_label.grid(row=1, column=0, columnspan=3, padx=10, pady=10)

button_frame = tk.Frame(root, padx=10, pady=10, bg="#F0F8FF")
button_frame.grid(row=2, column=0, columnspan=3)

start_button = tk.Button(
    button_frame, text="Start Reading", command=start_reading)
start_button.grid(row=0, column=0, pady=10, padx=10)

stop_button = tk.Button(button_frame, text="Stop", command=on_closing)
stop_button.grid(row=0, column=1, pady=10, padx=10)

root.bind('q', on_closing)
root.mainloop()
