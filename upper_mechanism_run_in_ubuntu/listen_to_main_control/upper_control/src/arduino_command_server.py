#!/usr/bin/env python
# this program get the msg sended from client ,which we manually input ,and then send it to arduino

import rospy
from std_msgs.msg import Int8
import serial 
from time import sleep
from upper_control.srv import action,actionResponse 

'''
    The representations of actions:
    0 , standard position
    1 , taking basketball
    2 , throwing basketball
    3 , taking bowling
    4 , relasing bowling
'''
take_basketball_times = 0
take_bowling = 0

def Go_position(tasklist):
    for move in tasklist:
        ser.write(bytes(str(move), 'utf-8'))

    # check whether the task is done
    while len(tasklist) > 0:
        arduino_echo = ''
        while arduino_echo == '' :
            arduino_echo = ser.readline().decode('utf').strip()
            if int(arduino_echo) in tasklist:
                tasklist.remove(arduino_echo)
    
def taking_basketball():
    take_basketball_times = take_basketball_times + 1
    if(take_basketball_times == 1):
        # Pusher  up
        ser.write(bytes('33', 'utf-8'))
        sleep(5)
        ser.write(bytes('31', 'utf-8'))

        # downStepper backward 
        tasklist = [23]
        Go_position(tasklist)

        # Pusher down
        tasklist = [32]
        Go_position(tasklist)
        sleep(1)

        # Pusher up
        ser.write(bytes('33', 'utf-8'))
        sleep(5)
        ser.write(bytes('31', 'utf-8'))

    elif(take_basketball_times == 2):
        # Pusher down
        tasklist = [32]
        Go_position(tasklist)
        sleep(1)

        #Pusher up
        ser.write(bytes('33', 'utf-8'))
        sleep(5)
        ser.write(bytes('31', 'utf-8'))

    elif(take_basketball_times == 3):
        take_basketball_times = 0

        # Pusher down
        tasklist = [32]
        Go_position(tasklist)
        sleep(1)

        #Pusher up
        ser.write(bytes('33', 'utf-8'))
        sleep(5)
        ser.write(bytes('31', 'utf-8'))

# def throwing_basketball():
# def taking_bowling():
# def relasing_bowling():

def callback(request):
    actions = [0,1,2,3,4]
    if(request.request in actions): 
        if(request.request == 0):
            tasklist = [13,22,32]
            Go_position(tasklist)
        elif(request.request == 1):
            taking_basketball()
        # else if(request.request == 2):
        # else if(request.request == 3):
        # else if(request.request == 4):
        # if the request sended from client is qualified , send it to arduino
        # ser.write(bytes(str(request.request), 'utf-8'))

        # waiting the message from arduino
        # arduino_echo = ''
        # while arduino_echo == '' :
        #     arduino_echo = ser.readline().decode('utf').strip()
        
    else :
        print('Arduino :invalid command')
        request.request = -1
        
    # 回傳response 給 client
    return actionResponse(request.request) 


if __name__ == '__main__':

    rospy.init_node('upper_mechanism')
    
    # connect to arduino board
    global ser
    ser = serial.Serial('/dev/ttyUSB0',57600)
    ser.timeout = 3
    sleep(3)

    try:
        while True:

            # if client send a request ,call callback
            rospy.Service("action",action,callback) 
            rospy.spin()

    except rospy.ROSInterruptException:
        ser.close()
        print('\nend')
        exit()
