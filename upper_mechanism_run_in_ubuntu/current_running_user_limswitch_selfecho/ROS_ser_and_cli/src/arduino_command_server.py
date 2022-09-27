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

def callback(request):
    actions = [0,1,21,22,23,31,32,33,41,42,43,51,52,53]
    if(request.request in actions): 
        
        # if the request is qualified , send it to arduino
        ser.write(bytes(str(request.request), 'utf-8'))
        arduino_echo = ''
        while arduino_echo == '' :
            arduino_echo = ser.readline().decode('utf').strip()
        print("Arduino :" , arduino_echo)
    else :
        print('Arduino :invalid command')
        request.request = -1
        
    # return the response to client
    return actionResponse(request.request) 


if __name__ == '__main__':

    rospy.init_node('upper_mechanism')
    
    # connect to arduino board
    ser = serial.Serial('/dev/ttyUSB0',57600)
    ser.timeout = 3
    sleep(3)

    while True:
        try:
            # if the client send a request , call callback
            rospy.Service('upper_mechanism',action,callback) 
            rospy.spin()
            
        except rospy.ROSInterruptException:
            ser.close()
            print('\nend')
            exit()
