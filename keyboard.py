import serial
import time
import msvcrt

ser = serial.Serial('COM3',115200)
time.sleep(2)

while True:
    if msvcrt.kbhit():
        key = msvcrt.getch()
        ser.write(key)

