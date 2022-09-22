#!/usr/bin/env python
# this program get the msg sended from client ,which we manually input ,and then send it to arduino

import rospy
from std_msgs.msg import Int8
import serial 
from time import sleep
from upper_control.srv import action,actionResponse 
import threading 

'''
    The representations of actions:
    0 , standard position
        topStepper backward
        downStepper forward
        pusher down.start()
        pusher down.join()

    1 , taking basketball
        - first 
            pusher up
            sleep(3)
            downStepper backward.start()
            sleep(?)取決於取求仰角
            pusher stop
            downStepper backward.join()
            pusher down.start()
            pusher down.join()

        - second 
            pusher up
            sleep(3+?)
            pusher stop
            pusher down.start()
            pusher down.join()

        - third 
            pusher up
            sleep(3+?)
            pusher stop
            pusher down.start()
            pusher down.join()

    2 , throwing basketball

        pusher up
        sleep(?)
        pusher stop

        topStepper forward
        sleep(?)
        topStepper stop

        sleep(?) 緩衝

        topStepper forward
        sleep(?)
        topStepper stop

        sleep(?)緩衝

        downStepper forward.start()
        downStepper forward.join()

    3 , taking bowling
    4 , relasing bowling
'''
take_basketball_times = 0
take_bowling = 0


# def throwing_basketball():
# def taking_bowling():
# def relasing_bowling():

def callback(request):
    actions = [0,1,2,3,4]
    if(request.request in actions): 
        # if(request.request == 0):
        # elif(request.request == 1):
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
