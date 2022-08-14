import serial
import sys
import message as msg
import motor as mot
from select_com_port import select_com_port

select_com_port()
try:
    while True:
        mot.command()
        msg.read()  
except KeyboardInterrupt:
    mot.stop_all_motor()
    serial.close()
    print('\nend')
    exit()