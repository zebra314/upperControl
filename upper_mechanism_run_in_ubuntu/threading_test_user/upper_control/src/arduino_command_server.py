#!/usr/bin/env python
# this program get the msg sended from client ,which we manually input ,and then send it to arduino

#####################
### command guide ###
#####################

# topStepper  11 stop, 12 forward, 13 backward
# downStepper 21 stop, 22 forward, 23 backward
# Pusher      31 stop, 32 forward, 33 backward
# flywheel    41 stop, 42 shoot

# 0 , standard position
# 1 , taking basketball
# 2 , throwing basketball
# 3 , taking bowling
# 4 , relasing bowling

import rospy
from std_msgs.msg import Int8
import serial 
from time import sleep
from upper_control.srv import action,actionResponse 
import threading 


# The representations of actions:
# 0 , standard position
# 1 , taking basketball
# 2 , throwing basketball
#     pusher up
#     sleep(?)
#     pusher stop
#     topStepper forward
#     sleep(?)
#     topStepper stop
#     sleep(?) 緩衝
#     topStepper forward
#     sleep(?)
#     topStepper stop
#     sleep(?)緩衝
#     downStepper forward.start()
#     downStepper forward.join()
# 3 , taking bowling
# 4 , relasing bowling


# Programs about sending the msg to arduino and waiting the echo by using threading

def topStepperBackward():
    ser.write(bytes('13', 'utf-8'))
    print('done sending')
    arduino_echo = ''
    while arduino_echo != '13' : # waiting the message from arduino
        print('waiting echo')
        arduino_echo = ser.readline().decode('utf').strip()
topStepper_backward = threading.Thread(target = topStepperBackward) # creat thread

def downStepperForward():
    ser.write(bytes('22', 'utf-8'))
    print('done sending')
    arduino_echo = ''
    while arduino_echo != '22' : # waiting the message from arduino
        print('waiting echo')
        arduino_echo = ser.readline().decode('utf').strip()
downStepper_forward = threading.Thread(target = downStepperForward) # creat thread 

def downStepperBackward():
    ser.write(bytes('23', 'utf-8'))
    print('done sending')
    arduino_echo = ''
    while arduino_echo != '23' : # waiting the message from arduino
        print('waiting echo')
        arduino_echo = ser.readline().decode('utf').strip()
downStepper_backward = threading.Thread(target = downStepperBackward) # creat thread    

def pusherDown():
    ser.write(bytes('32', 'utf-8'))
    print('done sending')
    arduino_echo = ''
    while arduino_echo != '32' : # waiting the message from arduino
        print('waiting echo')
        arduino_echo = ser.readline().decode('utf').strip()
pusher_down = threading.Thread(target = pusherDown) # creat thread


# Program about the motion package

def standard_position():
    topStepper_backward.start() # topStepper backward
    downStepper_forward.start() # downStepper forward
    pusher_down.start() # pusher down
    topStepper_backward.join()
    downStepper_forward.join()
    pusher_down.join()
    
take_basketball_times = 0
def taking_basketball():
    global take_basketball_times 

    # take the first basketball
    if(take_basketball_times == 1):
        ser.write(bytes('33', 'utf-8')) # pusher up
        sleep(3)
        downStepper_backward.start() # downStepper backward
        sleep(2)
        ser.write(bytes('31', 'utf-8')) # pusher stop
        downStepper_backward.join()
        pusher_down.start()
        pusher_down.join()
        ser.write(bytes('33', 'utf-8')) # pusher up
        sleep(5)
        ser.write(bytes('31', 'utf-8')) # pusher stop

    # take the second and the third basketball
    elif(take_basketball_times == 2 or take_basketball_times == 3):
        pusher_down.start()
        pusher_down.join()
        ser.write(bytes('33', 'utf-8')) # pusher up
        sleep(5)
        ser.write(bytes('31', 'utf-8')) # pusher stop
        if(take_basketball_times == 3):
            take_basketball_times = 0

# def throwing_basketball():
# def taking_bowling():
# def relasing_bowling():

def callback(request):
    motion_pkg = [0, 1, 2, 3, 4]
    actions = [11, 12, 13, 21, 22, 23, 31, 32, 33, 41, 42]
    if(request.request in motion_pkg): 
        if(request.request == 0):
            standard_position()
            print('done standard_position')
        elif(request.request == 1):
            take_basketball_times = take_basketball_times + 1
            taking_basketball()
            print('done taking basketball')
        # elif(request.request == 2):
        # elif(request.request == 3):
        # elif(request.request == 4):  
    elif(request.request in actions):
        ser.write(bytes(str(request.request), 'utf-8'))
    else :
        print('invalid command')
        request.request = -1
        
    return actionResponse(request.request) # send the response to client


if __name__ == '__main__':

    rospy.init_node('upper_mechanism')

    # connect to arduino board
    ser = serial.Serial('/dev/ttyUSB0',57600)
    ser.timeout = 3
    sleep(3)
    print('arduino connected')
    
while True:
    try:
        # if client send a request ,call callback()
        rospy.Service("upper_mechanism",action,callback) 
        rospy.spin()

    except rospy.ROSInterruptException:
        ser.close()
        print('\nend')
        exit()
