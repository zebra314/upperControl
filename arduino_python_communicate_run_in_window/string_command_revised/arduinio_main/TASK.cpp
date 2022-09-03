//共兩個pusher 兩個DC 兩個Stepper
// a,b Stepper , c,d Pusher , e,f DC motor

#include "TASK.h"
#include "Arduino.h"

const byte a_CLK 2  // step
const byte a_CW 7   // direction
const byte b_ENA 11 
const byte b_IN1 9 
const byte b_IN2 10
const byte c_IN1 6  // speed
const byte c_IN2 5  // direction

TASK::TASK()
{   
    pinMode(a_CLK, OUTPUT);
    pinMode(a_CW, OUTPUT);
    pinMode(b_IN1, OUTPUT);
    pinMode(b_IN2, OUTPUT);
    pinMode(b_ENA, OUTPUT);
    pinMode(c_IN1, OUTPUT);
    pinMode(c_IN2, OUTPUT);
}

TASK::~TASK()
{ /*nothing to destruct*/
}


// directions not yet confirmed 
void TASK::a_task(int status)
{
    if (status == 1) // stop
    {
        digitalWrite(a_CLK, LOW);
    }
    else if (status == 2) // counterclockwise
    {
        digitalWrite(a_CW, LOW);
        digitalWrite(a_CLK, HIGH);
        delayMicroseconds(500);
        digitalWrite(a_CLK, LOW);
        delayMicroseconds(500);
    }
    else if (status == 3) // clockwise
    {
        digitalWrite(a_CW, HIGH);
        digitalWrite(a_CLK, HIGH);
        delayMicroseconds(500);
        digitalWrite(a_CLK, LOW);
        delayMicroseconds(500);
    }
}

void TASK::b_task(int status)
{
    if (status == 1) // stop
    {
        digitalWrite(b_IN1, LOW);
        digitalWrite(b_IN2, LOW);
        analogWrite(b_ENA, 0);
    }
    else if (status == 2) // 縮短
    {
        digitalWrite(b_IN1, HIGH);
        digitalWrite(b_IN2, LOW);
        analogWrite(b_ENA, 240);
    }
    else if (status == 3) // 伸長
    {
        digitalWrite(b_IN1, LOW);
        digitalWrite(b_IN2, HIGH);
        analogWrite(b_ENA, 240);
    }
}


// directions not yet confirmed 
void TASK::c_task(int status)
{
    if (status == 1) // stop
    {
        analogWrite(c_IN1, 0);
        digitalWrite(c_IN2, HIGH);
    }
    else if (status == 2) // counterclockwise
    {
        analogWrite(c_IN1, 150);
        digitalWrite(c_IN2, HIGH);
    }
    else if (status == 3) // clockwise
    {
        analogWrite(c_IN1, 150);
        digitalWrite(c_IN2, LOW);
    }
}