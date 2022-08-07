import serial
from time import sleep
import sys

# read the message from arduino
def read():
    message = ""
    while serial.in_waiting:

        #rstrip() remove the blank
        inbyte = serial.readline().decode('utf').rstrip()

        #ascii to char and then combine to a string
        message += chr(int(inbyte)) 
    print ("Message from arduino: " + message)

# send message to arduino
def send(string):
    serial.write(string.encode('utf-8'))
    print('Message from user: ' + string)
    sleep(2)

