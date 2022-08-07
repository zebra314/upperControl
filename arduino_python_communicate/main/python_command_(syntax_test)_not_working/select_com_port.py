import serial
from time import sleep
import sys

# connect com port manually
def select_com_port():
    error = 1
    while(error == 1):
        try:
            val = input("Select Port :COM")
            COM_PORT = 'COM'+ str(val)
            BAUD_RATES = 9600
            serial = serial.Serial(COM_PORT, BAUD_RATES)
            error = 0
        except KeyboardInterrupt:
            print('\nend')    
            exit()
        except:
            print("failed to connect " + COM_PORT)
    print(COM_PORT + " connected")
    sleep(2)