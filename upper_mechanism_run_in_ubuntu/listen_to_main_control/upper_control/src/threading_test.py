import threading
from time import sleep

def task():
    print('task start')
    sleep(3)
    print('task end')

def count():
    while(threading.active_count()!= 2):
        print(str(threading.active_count()))

t = threading.Thread(target = task)
c = threading.Thread(target = count)
print('competition start')
sleep(3)
t.start()
c.start()
t.join()
c.join()
sleep(3)
print('competition end')