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
    1 , taking basketball
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

# topStepper 11, 12, 13退 
# downStepper 21, 22, 23退
# Pusher 31, 32, 33伸
# flywheel 41, 42

take_basketball_times = 0

def pusherDown():
    ser.write(bytes('32', 'utf-8'))
    arduino_echo = ''
    while arduino_echo != '32' :
        arduino_echo = ser.readline().decode('utf').strip()
        
pusherDown = threading.Thread(target = pusherDown) # creat pusher_down thread

def standard_position():
    ser.write(bytes('13', 'utf-8')) # topStepper backward
    ser.write(bytes('22', 'utf-8')) # downStepper forward
    pusherDown.start()
    pusherDown.join()
    
def taking_basketball():
    global take_basketball_times 
    if(take_basketball_times == 1):
        # pusher up
        # sleep(3)
        # downStepper backward.start()
        # sleep(?)取決於取求仰角
        # pusher stop
        # downStepper backward.join()
        # pusher down.start()
        # pusher down.join()
    elif(take_basketball_times == 2):
        # pusher up
        # sleep(3+?)
        # pusher stop
        # pusher down.start()
        # pusher down.join()
    elif(take_basketball_times == 3):
        take_basketball_times =  0
        # pusher up
        # sleep(3+?)
        # pusher stop
        # pusher down.start()
        # pusher down.join()





# def throwing_basketball():
# def taking_bowling():
# def relasing_bowling():

def callback(request):
    if(request.request == 0):
        standard_position()
    elif(request.request == 1):
        take_basketball_times = take_basketball_times + 1
        taking_basketball()
    # elif(request.request == 2):
    # elif(request.request == 3):
    # elif(request.request == 4):
        # if the request sended from client is qualified , send it to arduino
        # ser.write(bytes(str(request.request), 'utf-8'))

        # waiting the message from arduino
        # arduino_echo = ''
        # while arduino_echo == '' :
        #     arduino_echo = ser.readline().decode('utf').strip()

        
    # 回傳response 給 client
    return actionResponse(request.request) 


if __name__ == '__main__':

    rospy.init_node('upper_mechanism')
    
    # connect to arduino board
    global ser
    ser = serial.Serial('/dev/arduino_control',57600)
    ser.timeout = 3
    sleep(3)

    try:
        while True:
            # if client send a request ,call callback
            rospy.Service("upper_mechanism",action,callback) # action名稱太爛要改
            rospy.spin()

    except rospy.ROSInterruptException:
        ser.close()
        print('\nend')
        exit()