#!/usr/bin/env python
# this program get the msg sended from client ,which we manually input ,and then send it to arduino

#####################
### command guide ###
#####################

# topStepper  51 stop, 52 forward, 53 backward
# downStepper 61 stop, 62 forward, 63 backward
# Pusher      71 stop, 72 forward, 73 backward
# flywheel    81 stop, 82 shoot

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


def callback(request):
    motion_pkg = [0,1,2,3,4]
    actions = [51,52,53,61,62,63,71,72,73,81,82]
    if(request.request in motion_pkg): 
        print('valid command')
        ser.write(bytes(str(request.request), 'utf-8'))

        print('msg sended , waiting arduino echo...')
        arduino_echo = ''
        while arduino_echo == '' :
            arduino_echo = ser.readline().decode('utf').strip()
        print("Arduino :" , arduino_echo)
        
    elif(request.request in actions):
        print('valid command')
        ser.write(bytes(str(request.request), 'utf-8')) # if the request is qualified , send it to arduino
        
    else:
        print('invalid command')
        request.request = -1

    return actionResponse(request.request) # return the response to client


if __name__ == '__main__':

    rospy.init_node('upper_mechanism')
    
    # connect to arduino board
    ser = serial.Serial('/dev/ttyUSB0',57600)
    ser.timeout = 3
    sleep(3)
    print('arduino connected')

    while True:
        try:
            print('waiting command...')
            # if the client send a request , call callback
            rospy.Service('upper_mechanism',action,callback) 
            rospy.spin()
            
        except rospy.ROSInterruptException:
            ser.close()
            print('\nend')
            exit()
