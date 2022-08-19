import serial
from time import sleep
import sys

# read the message from arduino
def read_arduino():
    while serial.in_waiting:
        message = serial.readline()
        showvalue = message.decode('utf').rstrip('\n')
    print ('Message from arduino: ' + showvalue)

# send message to arduino
def send_message(string):
    serial.write(bytes(string,'utf-8'))
    print('Message from user: ' + string)
    sleep(2)

# Stepper = 1 , Pusher = 2 , Brushless DC motor = 3 , test servo = 4
# 1= stop , 2 = counterclockwise , 3 = clockwise 
# motor type (1,2,3) + action (1,2,3) 
# ex: 11 , 21 , 33
def command():
    error = 1
    actions = ['11','12','13','21','22','23','31','32','33','41','42','43']
    while(error == 1):
        command = input("command: ").lower()
        if(command in actions):
            send_message(command)
            error =0
        else:
            print('invalid command')

def stop_all_motor():
    stops = ['11','21','31','41']  
    for motor in stops:
        serial.write(motor.encode('utf-8'))
        sleep(2)

# select the COM port manually
def select_COMport():
    global serial
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

if __name__ == '__main__':
    select_COMport()
    try:
        while True:
            command()
            read_arduino()  
    except KeyboardInterrupt:
        stop_all_motor()
        serial.close()
        print('\nend')
        exit()