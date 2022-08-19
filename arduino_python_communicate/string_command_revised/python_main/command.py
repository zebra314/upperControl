from time import sleep
import message as msg
from main import ArduinoSerial

# Stepper = a , Pusher = b , Brushless DC motor = c
# 1= stop , 2 = counterclockwise , 3 = clockwise
# motor type (a,b,c) + action (1,2,3)
# ex: a1 , b1 , c3
def Command():
    error = 1
    actions = ['a1', 'a2', 'a3', 'b1', 'b2',
               'b3', 'c1', 'c2', 'c3', 't1', 't2', 't3']
    while (error == 1):
        command = input("command: ").lower()
        if (command in actions):
            msg.SendMessage(command)
            error = 0
        else:
            print('invalid command')

# stop all motor
def StopAll():
    stops = ['a1', 'b1', 'c1', 't1']
    for motor in stops:
        ArduinoSerial.write(bytes(motor, 'utf-8'))
        sleep(2)