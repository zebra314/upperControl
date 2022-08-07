import serial
from time import sleep
import sys

# read the message from arduino
def read_arduino():
    message = ""
    while serial.in_waiting:

        #rstrip() to remove the blank
        inbyte = serial.readline().decode('utf').rstrip()

        #ascii to char and then combine to a string
        message += chr(int(inbyte)) 
    print ("Message from arduino: " + message)

# send message to arduino
def send_message(string):
    serial.write(string.encode('utf-8'))
    print('Message from user: ' + string)
    sleep(2)

# Stepper = a , Pusher = b , Brushless DC motor = c
# 1= stop , 2 = counterclockwise , 3 = clockwise 
# motor type (a,b,c) + action (1,2,3) 
# ex: a1 , b1 , c3
def command():
    actions = ['a1','a2','a3','b1','b2','b3','c1','c2','c3']
    command = input("command: ").lower()
    if(command in actions):
        send_message(command)
    else:
        print('wrong')

def stop_all_motor():
    stops = ['a1','b1','c1']  
    for motor in stops:
        serial.write(motor.encode('utf-8'))
        sleep(2)

# connect to the ardunio manually
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

try:
    while True:
        command()
        read_arduino()  
except KeyboardInterrupt:
    stop_all_motor()
    serial.close()
    print('\nend')
    exit()