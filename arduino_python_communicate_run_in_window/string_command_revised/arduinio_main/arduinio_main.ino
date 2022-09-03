#include"TASK.h"

void setup()
{
    Serial.begin(57600);
}

TASK motor;

void action(String message)
{
    char motor_type = message[0];
    char motor_status = message[1];

    switch (motor_type)
    {
    case 'a':
        motor.a_task(int(motor_status - '0'));
    case 'b':
        motor.b_task(int(motor_status - '0'));
    case 'c':
        motor.c_task(int(motor_status - '0'));
    case 't':
        t_task(int(motor_status - '0'));
    }
}

String message;
void loop()
{
    if (Serial.available() > 0)
    {
        message = Serial.readString();
        Serial.println(message);
    }
    action(message);
}