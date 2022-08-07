import serial
from time import sleep
import sys

#choose the serial port of arduino
error =1
while(error==1):
    try:
        val = input("Select Port :COM")
        COM_PORT = 'COM'+ str(val)
        BAUD_RATES = 9600
        ser = serial.Serial(COM_PORT, BAUD_RATES)
        error =0
    except:
        print("Could not open the port")
print("The port open successfully")

#read the data from arduino 
try:
    while True:
        print(ser.readline())
        while ser.in_waiting:
            try:
                mcu_feedback = ser.readline().decode()  
                print('reacting.....', mcu_feedback)
            except:
                print("serial read error")

except KeyboardInterrupt:
    ser.close()
    print('seeu')