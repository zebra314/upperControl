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

#enter the command and send it to arduino
try:
    while True:
        choice = input('enter').lower()
        if choice == '1':
            print('forward')
            ser.write(b'1')  
            sleep(0.5)              
        elif choice == '2':
            print('backward')
            ser.write(b'2')
            sleep(0.5)
        elif choice == '3':
            print('turn left')
            ser.write(b'3')
            sleep(0.5)
        elif choice == '4':
            print('turn right')
            ser.write(b'4') 
            sleep(0.5)
        elif choice == '0':
            ser.close()
            print('seeu ')
            sys.exit()
        else:
            print('wrong')
            ser.write(b'7')
            sleep(0.5)
        
        while ser.in_waiting:
            try:
                mcu_feedback = ser.readline().decode()  
                print('reacting.....', mcu_feedback)
            except:
                print("serial read error")

#quit
except KeyboardInterrupt:
    ser.close()
    print('seeu')