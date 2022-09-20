#!/usr/bin/env python
import serial 
from time import sleep

if __name__ == '__main__':

    ser = serial.Serial('/dev/ttyUSB0',57600)
    ser.timeout = 3
    sleep(3)
    arduino_echo = "Arduino :" + ser.readline().decode('utf').strip()
    print(arduino_echo) 
    
    try:
        while True:
            msg = input("command: ").lower()
            actions = ['t1','t2','t3','d1','d2','d3','p1','p2','p3','f1','f2','f3']
            if(msg in actions):
                ser.write(bytes(msg, 'utf-8'))
                arduino_echo = "Arduino :" + ser.readline().decode('utf').strip()
                print(arduino_echo)
            else :
                print('invalid command')
    except KeyboardInterrupt:
        ser.close()
        print('\nend')
        exit()