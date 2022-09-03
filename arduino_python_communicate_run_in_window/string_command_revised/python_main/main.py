import sys
import serial
from time import sleep
import message as msg
import command as com

# select the COM port manually
def SelectCOMport():
    global ArduinoSerial
    error = 1
    while (error == 1):
        try:
            val = input("Select Port :COM")
            COM_PORT = 'COM' + str(val)
            BAUD_RATES = 57600
            ArduinoSerial = serial.Serial(COM_PORT, BAUD_RATES)
            error = 0
        except KeyboardInterrupt:
            print('\nend')
            exit()
        except:
            print("failed to connect " + COM_PORT)
    print(COM_PORT + " connected")
    sleep(2)

if __name__ == '__main__':
    SelectCOMport()
    try:
        while True:
            com.Command()
            msg.ReadArduino()
    except KeyboardInterrupt:
        com.StopAll()
        ArduinoSerial.close()
        print('\nend')
        exit()