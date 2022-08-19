// a,e Stepper , b 推桿 , c,d DC motor
const byte a_CLK = 2; // step
const byte a_CW  = 7;  // direction
const byte e_CLK = 4; // step
const byte e_CW  = 21;  // direction
const byte b_ENA = 11;
const byte b_IN1 = 9;
const byte b_IN2 = 12;
const byte c_IN1 =  8; //speed
const byte c_IN2 = 44; //direction
const byte d_IN1 = 10; //speed
const byte d_IN2 = 50; //direction

void setup()
{
    Serial.begin(57600);
    pinMode(a_CLK, OUTPUT);
    pinMode(a_CW,  OUTPUT);
    pinMode(e_CLK, OUTPUT);
    pinMode(e_CW,  OUTPUT);
    pinMode(b_ENA, OUTPUT);
    pinMode(b_IN1, OUTPUT);
    pinMode(b_IN2, OUTPUT);
    pinMode(c_IN1, OUTPUT);
    pinMode(c_IN2, OUTPUT);
    pinMode(d_IN1, OUTPUT);
    pinMode(d_IN2, OUTPUT);
}

void a_task(int status)  //Stepper 1
{
    if (status == 1) // stop
    {
        digitalWrite(a_CLK, LOW);
    }
    else if (status == 2) // counterclockwise(?
    {
        digitalWrite(a_CW, LOW);
        digitalWrite(a_CLK, HIGH);
        delayMicroseconds(500);
        digitalWrite(a_CLK, LOW);
        delayMicroseconds(500);
    }
    else if (status == 3) // clockwise(?
    {
        digitalWrite(a_CW, HIGH);
        digitalWrite(a_CLK, HIGH);
        delayMicroseconds(500);
        digitalWrite(a_CLK, LOW);
        delayMicroseconds(500);
    }
}

void e_task(int status) //Stepper 2
{
    if (status == 1) // stop
    {
        digitalWrite(e_CLK, LOW);
    }
    else if (status == 2) // up
    {
        digitalWrite(e_CW, LOW);
        digitalWrite(e_CLK, HIGH);
        delayMicroseconds(700);
        digitalWrite(e_CLK, LOW);
        delayMicroseconds(700);
    }
    else if (status == 3) // downward
    {
        digitalWrite(e_CW, HIGH);
        digitalWrite(e_CLK, HIGH);
        delayMicroseconds(700);
        digitalWrite(e_CLK, LOW);
        delayMicroseconds(700);
    }
}

void b_task(int status) // pusher 1
{
    if (status == 1) // stop
    {
        digitalWrite(b_IN1, LOW);
        digitalWrite(b_IN2, LOW);
        analogWrite(b_ENA, 0);
    }
    else if (status == 2) //縮短
    {
        digitalWrite(b_IN1, HIGH);
        digitalWrite(b_IN2, LOW);
        analogWrite(b_ENA, 240);
    }
    else if (status == 3) //伸長
    {
        digitalWrite(b_IN1, LOW);
        digitalWrite(b_IN2, HIGH);
        analogWrite(b_ENA, 240);
    }
}

// directions not yet confirmed
void c_task(int status,int speeed) //DC motor 1
{
    if (status == 1) // stop
    {
        analogWrite(c_IN1, 0);
        digitalWrite(c_IN2, HIGH);
    }
    else if (status == 2) // counterclockwise(?
    {
        analogWrite(c_IN1, speeed);
        digitalWrite(c_IN2, HIGH);
    }
    else if (status == 3) // clockwise(?
    {
        analogWrite(c_IN1, speeed);
        digitalWrite(c_IN2, LOW);
    }
}

void d_task(int status,int speeed)  //DC motor 2
{
    if (status == 1) // stop
    {
        analogWrite(d_IN1, 0);
        digitalWrite(d_IN2, LOW);
    }
    else if (status == 2) // counterclockwise(?
    {
        analogWrite(d_IN1, speeed);
        digitalWrite(d_IN2, HIGH);
    }
    else if (status == 3) // clockwise(?
    {
        analogWrite(d_IN1, speeed);
        digitalWrite(d_IN2, LOW);
    }
}

/* 51 102 153 204 255
1 2 3 4 5*/
void action(String message)
{
    char motor_type = message[0];
    char motor_status = message[1];
    char motor_speed = message[2];
    int speed;
    if (motor_speed == '\0')
    {
        speed = 150 ;
    }
    else
    {
        speed = map(int(motor_speed - '0'),1,5,0,255);
    }

    switch (motor_type)
    {
    case 'a':
        a_task(int(motor_status - '0'));
    case 'e':
        e_task(int(motor_status - '0'));
    case 'b':
        b_task(int(motor_status - '0'));
    case 'c':
        c_task(int(motor_status - '0'),speed);
    case 'd':
        d_task(int(motor_status - '0'),speed);
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
    if(message == "s2") //上 微調前
    {
       for(int i =0;i<50;i++)
       {
         digitalWrite(a_CW, LOW);
        digitalWrite(a_CLK, HIGH);
        delayMicroseconds(700);
        digitalWrite(a_CLK, LOW);
        delayMicroseconds(700);
       }
 
        message = "";
    }else if(message == "s3") //上 微調後
    {
       for(int i =0;i<50;i++)
       {
        digitalWrite(a_CW, HIGH);
        digitalWrite(a_CLK, HIGH);
        delayMicroseconds(700);
        digitalWrite(a_CLK, LOW);
        delayMicroseconds(700);
       }
        message  = "" ;
    }else
    {
      action(message);
    }
    
    
}
