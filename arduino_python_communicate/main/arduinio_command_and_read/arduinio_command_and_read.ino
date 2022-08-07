//a Stepper
//b Pusher
//c DC motor 
#define a_CLK 2 //step
#define a_CW 7  //direction
#define b_ENA 8
#define b_IN1 9
#define b_IN2 10
#define c_IN1   //speed
#define c_IN2   //direction

char a_status; //Stepper
char b_status; //Pusher
char c_status; //DC motor

/*status    1 == stop
            2 == counterclockwise
            3 == clockwise */

void setup()
{
    Serial.begin(9600);
    pinMode(a_CLK, OUTPUT);
    pinMode(a_CW, OUTPUT);
    pinMode(b_ENA,OUTPUT);
    pinMode(b_IN1,OUTPUT);
    pinMode(b_IN2,OUTPUT);
}

void action(String message)
{
    switch(message[0]) //motor type
    {
        case 'a':
            a_status = message[1];
        case 'b':
            b_status = message[1];
        case 'c':
            c_status = message[1];
    }
}

// repeat
// directions not confirm yet
void a_task(char status)
{
    if(status == '1') //stop
    {
        digitalWrite(a_CLK, LOW);
    }
    else if (status == '2') //counterclockwise
    {
        digitalWrite(a_CW, LOW);
        digitalWrite(a_CLK, HIGH);
        delayMicroseconds(500);
        digitalWrite(a_CLK, LOW);
        delayMicroseconds(500);
    }
    else if (status == '3') //clockwise
    {
        digitalWrite(a_CW, HIGH);
        digitalWrite(a_CLK, HIGH);
        delayMicroseconds(500);
        digitalWrite(a_CLK, LOW);
        delayMicroseconds(500);
    }
}

void b_task(char status )
{
    if(status == '1') //stop
    {
        digitalWrite(b_IN1,LOW);
        digitalWrite(b_IN2,LOW);
        analogWrite(b_ENA,0);
    }
    else if (status == '2') //counterclockwise
    {
        digitalWrite(b_IN1,HIGH);
        digitalWrite(b_IN2,LOW);
        analogWrite(b_ENA,100);
    }
    else if (status == '3') //clockwise
    {
        digitalWrite(b_IN1,LOW);
        digitalWrite(b_IN2,HIGH);
        analogWrite(b_ENA,100);
    }
}

void c_task(char status)
{
    if(status == '1') //stop
    {

    }
    else if (status == '2') //counterclockwise
    {

    }
    else if (status == '3') //clockwise
    {

    }
}

void loop()
{
    while (Serial.available() > 0)
    {
        String message;
        char inbyte;
        message = Serial.read();
        Serial.println(message); //python will receive the message in ascii
        action(message);
    } 
    a_task(a_status);
    b_task(b_status);
    c_task(c_status);
}