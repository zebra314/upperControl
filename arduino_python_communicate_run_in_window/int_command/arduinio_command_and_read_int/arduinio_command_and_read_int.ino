#include<Servo.h> 
//a Stepper , b Pusher , c DC motor 
#define a_CLK 2 //step
#define a_CW 7 //direction
#define b_ENA 11
#define b_IN1 9
#define b_IN2 10
#define c_IN1 6 //speed
#define c_IN2 5 //direction

Servo myservo;

void setup()
{
    Serial.begin(9600);
    pinMode(a_CLK, OUTPUT);
    pinMode(a_CW, OUTPUT);
    pinMode(b_ENA,OUTPUT);
    pinMode(b_IN1,OUTPUT);
    pinMode(b_IN2,OUTPUT);
    myservo.attach(3); 
}

// directions not confirm yet
void a_task(int status)
{
    if(status == 1) //stop
    {
        digitalWrite(a_CLK, LOW);
    }
    else if (status == 2) //counterclockwise
    {
        digitalWrite(a_CW, LOW);
        digitalWrite(a_CLK, HIGH);
        delayMicroseconds(500);
        digitalWrite(a_CLK, LOW);
        delayMicroseconds(500);
    }
    else if (status == 3 ) //clockwise
    {
        digitalWrite(a_CW, HIGH);
        digitalWrite(a_CLK, HIGH);
        delayMicroseconds(500);
        digitalWrite(a_CLK, LOW);
        delayMicroseconds(500);
    }
}

void b_task(int status )
{
    if(status == 1) //stop
    {
        digitalWrite(b_IN1,LOW);
        digitalWrite(b_IN2,LOW);
        analogWrite(b_ENA,0);
    }
    else if (status == 2) //縮短
    {
        digitalWrite(b_IN1,HIGH);
        digitalWrite(b_IN2,LOW);
        analogWrite(b_ENA,200);
    }
    else if (status == 3) //伸長
    {
        digitalWrite(b_IN1,LOW);
        digitalWrite(b_IN2,HIGH);
        analogWrite(b_ENA,200);
    }
}

void c_task(int status)
{
    if(status == 1) //stop
    {

    }
    else if (status == 2) //counterclockwise
    {

    }
    else if (status == 3) //clockwise
    {

    }
}

void t_task(int status)
{
    if(status == 1) //stop
    {
        myservo.write(90);
    }
    else if (status == 2) 
    {
        myservo.write(0);
    }
    else if (status == 3) 
    {
        myservo.write(180);
    }
}

void action(int  message)
{
    switch(message / 10 ) //馬達類型(message的十位數)
    {
        case 1:
            a_task(message % 10); //動作(message的個位數)
        case 2:
            b_task(message % 10);
        case 3:
            c_task(message % 10);
        case 4:
            t_task(message % 10);
    }
}

void loop()
{
    if (Serial.available() > 0 )
    {
        int  message;
        message = Serial.readString().toInt();
        Serial.println(message); 
        action(message);
    } 
}