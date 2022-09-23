import threading
from time import sleep

def a(char):
    if(char == 'a'+ '\n'):
        print('done'+ '\n')
    else :
        print('a fail'+ '\n')
    for i in range (1,10):
        sleep(0.5)
        print(str(i) + 'a'+ '\n')

def b(char):
    if(char == 'b'):
        print('done' + '\n')
    else :
        print('b fail')
    for i in range (1,10):
        sleep(0.5)
        print(str(i) + 'b'+ '\n')

while True:
    av = input()
    a1 = threading.Thread(target = a, args = (av,))
    b1 = threading.Thread(target = b, args = (av,))

    a1.start()
    b1.start()
    a1.join() # 並非卡在這邊等他執行完
    b1.join()
    print('finally')

