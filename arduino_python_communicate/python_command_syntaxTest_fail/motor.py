import serial
from time import sleep
import sys
import message as msg

# stepper a ,pusher b ,無刷直流 c
# 正轉 3 ,反轉 2 ,停 1  
# motor type (a,b,c) + action (1,2,3) 
# ex: a1 a2 a3
def command():
    actions = ['a1','a2','a3','b1','b2','b3','c1','c2','c3']
    command = input("command: ").lower()
    if(command in actions):
        msg.send(command)
    else:
        print('wrong')

def stop_all_motor():
    stops = ['a1','b1','c1']  
    for motor in stops:
        serial.write(motor.encode('utf-8'))
        sleep(2)