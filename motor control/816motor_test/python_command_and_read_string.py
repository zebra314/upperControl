import serial
from time import sleep
import sys

# read the message from arduino
def read_arduino():
    while serial.in_waiting:
        # the return and new line characters \r\n are sent by the Arduino because
        # the potentiometer.ino sketch used the Serial.println() function,
        # which prints a new line each time the data is printed out.
        # ser.readline() returns a byte string: b'481\r\n'
        # .decode() converts the byte string to a Python string: '481\r\n'
        # .strip() removes the \r\n characters from the end of the Python string: '481'
        message = serial.readline().decode('utf').strip()
    print('Message from arduino: ' + message)

# send message to arduino
def send_message(string):
    serial.write(bytes(string, 'utf-8'))
    print('Message from user: ' + string)
    sleep(2)

# Stepper = a , Pusher = b , Brushless DC motor = c
# 1= stop , 2 = counterclockwise , 3 = clockwise
# motor type (a,b,c) + action (1,2,3)
# ex: a1 , b1 , c3
def command():
    error = 1
    actions = ['a1', 'a2', 'a3','e1', 'e2', 'e3', 'b1', 'b2','b3', 
                'c1', 'c2', 'c21','c22','c23','c24','c25', 'c3','c31','c32','c33','c34','c35', 
                'd1', 'd2', 'd21','d22','d23','d24','d25', 'd3','d31','d32','d33','d34','d35',]
    while (error == 1):
        command = input("command: ").lower()
        if (command in actions):
            send_message(command)
            error = 0
        else:
            print('invalid command')

# stop all motor
def stop_all_motor():
    stops = ['a1','e1' ,'b1', 'c1', 'd1']
    for motor in stops:
        serial.write(bytes(motor, 'utf-8'))
        sleep(2)
        

serial = serial.Serial('COM6', 57600)

if __name__ == '__main__':
    try:
        while True:
            command()
            read_arduino()
    except KeyboardInterrupt:
        stop_all_motor()
        serial.close()
        print('\nend')
        exit()