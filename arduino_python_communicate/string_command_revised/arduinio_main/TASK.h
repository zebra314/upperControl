#ifndef TASK_H
#define TASK_H
#include "Arduino.h"

class TASK
{
    private:
        byte pin; //控制led使用的引腳

    public:
        TASK();  //建構函式
        ~TASK(); //解構函式
        void a_task(int status);
        void b_task(int status);
        void c_task(int status);
        void t_task(int status);
};

#endif