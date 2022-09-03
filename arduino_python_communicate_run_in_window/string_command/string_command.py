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


# topStepper = t , downStepper = d , Pusher = p , flywheel  = f
# 1= stop , 2 , 3
# motor type (t,d,p,f) + action (1,2,3)
# ex: t1 , d1 , f3
def command():
    error = 1
    actions = ['t1','t2','t3','d1','d2','d3','p1','p2','p3','f1','f2','f3']
    while (error == 1):
        command = input("command: ").lower()
        if (command in actions):
            send_message(command)
            error = 0
        else:
            print('invalid command')

# stop all motor
def stop_all_motor():
    stops = ['t1','d1' ,'p1', 'f1']
    for motor in stops:
        serial.write(bytes(motor, 'utf-8'))
        sleep(2)
        
# select the COM port manually
def select_COMport():
    global serial
    error = 1
    while (error == 1):
        try:
            #val = input("Select Port (COM#, usbtty#) :") # 測試時方便先註解掉
            #COM_PORT = str(val)
            COM_PORT = 'COM6'
            BAUD_RATES = 57600
            serial = serial.Serial(COM_PORT, BAUD_RATES)
            error = 0
        except KeyboardInterrupt:
            print('\nend')
            exit()
        except:
            print("failed to connect " + COM_PORT)
    print(COM_PORT + " connected")
    sleep(2)

if __name__ == '__main__':
    select_COMport()
    try:
        while True:
            command()
            read_arduino()
    except KeyboardInterrupt:
        stop_all_motor()
        serial.close()
        print('\nend')
        exit()

# d3 下網後
# t3 上網後