from time import sleep
from main import ArduinoSerial

# read the message from arduino
def ReadArduino():
    while ArduinoSerial.in_waiting:
        # the return and new line characters \r\n are sent by the Arduino because
        # the potentiometer.ino sketch used the Serial.println() function,
        # which prints a new line each time the data is printed out.
        # ser.readline() returns a byte string: b'481\r\n'
        # .decode() converts the byte string to a Python string: '481\r\n'
        # .strip() removes the \r\n characters from the end of the Python string: '481'
        message = ArduinoSerial.readline().decode('utf').strip()
    print('Message from arduino: ' + message)

# send message to arduino
def SendMessage(string):
    ArduinoSerial.write(bytes(string, 'utf-8'))
    print('Message from user: ' + string)
    sleep(2)

