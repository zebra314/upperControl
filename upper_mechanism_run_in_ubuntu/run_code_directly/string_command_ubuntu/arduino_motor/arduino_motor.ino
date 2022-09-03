// topStepper ,  downStepper
const byte topStepper_CLK = 52; // step 
const byte topStepper_CW  = 50;  // direction
const byte downStepper_CLK = 44; // step
const byte downStepper_CW  = 46;  // direction

// Pusher
const byte Pusher_ENA = 11;
const byte Pusher_IN1 = 9;
const byte Pusher_IN2 = 12;
 
// rDCmotor 右馬達,  lDCmotor 左馬達 => flywheel
const byte rDCmotor_IN1 = 3; //speed 
const byte rDCmotor_IN2 = 24; //direction
const byte lDCmotor_IN1 = 2; //speed 
const byte lDCmotor_IN2 = 22; //direction

String message;
int topStepper_status;
int downStepper_status;
int Pusher_status;
int flywheel_status;

void setup()
{
    Serial.begin(57600);
    pinMode(topStepper_CLK, OUTPUT);
    pinMode(topStepper_CW,  OUTPUT);
    pinMode(downStepper_CLK, OUTPUT);
    pinMode(downStepper_CW,  OUTPUT);
    pinMode(Pusher_ENA, OUTPUT);
    pinMode(Pusher_IN1, OUTPUT);
    pinMode(Pusher_IN2, OUTPUT);
    pinMode(rDCmotor_IN1, OUTPUT);
    pinMode(rDCmotor_IN2, OUTPUT);
    pinMode(lDCmotor_IN1, OUTPUT);
    pinMode(lDCmotor_IN2, OUTPUT);
    Serial.print ("Ready...\n");
}

void topStepper_task(int status)  //Stepper 上
{
    if (status == 1) // stop
    {
        digitalWrite(topStepper_CLK, LOW);
    }
    else if (status == 2) //出 
    {
        digitalWrite(topStepper_CW, LOW);
        digitalWrite(topStepper_CLK, HIGH);
        delayMicroseconds(700);
        digitalWrite(topStepper_CLK, LOW);
        delayMicroseconds(700);
    }
    else if (status == 3) //進
    {
        digitalWrite(topStepper_CW, HIGH);
        digitalWrite(topStepper_CLK, HIGH);
        delayMicroseconds(700);
        digitalWrite(topStepper_CLK, LOW);
        delayMicroseconds(700);
    }
}

void downStepper_task(int status) //Stepper 下
{
    if (status == 1) // stop
    {
        digitalWrite(downStepper_CLK, LOW);
    }
    else if (status == 2) // counterclockwise(?
    {
        digitalWrite(downStepper_CW, HIGH);
        digitalWrite(downStepper_CLK, HIGH);
        delayMicroseconds(500);
        digitalWrite(downStepper_CLK, LOW);
        delayMicroseconds(500);
    }
    else if (status == 3) // clockwise(?
    {

        digitalWrite(downStepper_CW, LOW);
        digitalWrite(downStepper_CLK, HIGH);
        delayMicroseconds(500);
        digitalWrite(downStepper_CLK, LOW);
        delayMicroseconds(500);
    }
}

/*void s_task(int status)
{
    if (status == 3)  //上 微調後
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
    }
    else if(status == 2) //上 微調前
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
    }
}*/

void Pusher_task(int status) // pusher 1
{
    if (status == 1) // stop
    {
        digitalWrite(Pusher_IN1, LOW);
        digitalWrite(Pusher_IN2, LOW);
        analogWrite(Pusher_ENA, 0);
    }
    else if (status == 2) //縮短
    {
        digitalWrite(Pusher_IN1, HIGH);
        digitalWrite(Pusher_IN2, LOW);
        analogWrite(Pusher_ENA, 240);
    }
    else if (status == 3) //伸長
    {
        digitalWrite(Pusher_IN1, LOW);
        digitalWrite(Pusher_IN2, HIGH);
        analogWrite(Pusher_ENA, 240);
    }
}

#define speedIn 180
#define speedOut 180
void flywheel_task(int status) //flywheel
{
    if (status == 1) // stop
    {
        analogWrite(rDCmotor_IN1, 0);
        digitalWrite(rDCmotor_IN2, LOW);
        analogWrite(lDCmotor_IN1, 0);
        digitalWrite(lDCmotor_IN2, LOW);
    }
    else if (status == 2) // 噴
    {
        analogWrite(rDCmotor_IN1, speedOut);
        digitalWrite(rDCmotor_IN2, LOW);
        analogWrite(lDCmotor_IN1, speedOut);
        digitalWrite(lDCmotor_IN2, HIGH);
    }
    else if (status == 3) // 吸
    {
        analogWrite(rDCmotor_IN1, speedIn);
        digitalWrite(rDCmotor_IN2, HIGH);
        analogWrite(lDCmotor_IN1, speedIn);
        digitalWrite(lDCmotor_IN2, LOW);
    }
}

void action(String message)
{
    char motor_type = message[0];
    char motor_status = message[1];
    switch (motor_type)
    {
        case 't': //上
            topStepper_status = int(motor_status - '0');
            break;
        case 'd': //下
            downStepper_status = int(motor_status - '0');
            break;
        case 'p': //推桿
            Pusher_status = int(motor_status - '0');
            break;
        case 'f': //飛輪
            flywheel_status = int(motor_status - '0');  
            break; 
        /*case 's': //上微調
            s_task(int(motor_status - '0')); 出包*/
    }
}

void motorMove()
{
    topStepper_task(topStepper_status);
    downStepper_task(downStepper_status);
    Pusher_task(Pusher_status);
    flywheel_task(flywheel_status); 
}

void loop()
{
    if (Serial.available() > 0)
    {
        message = "arduino :" + Serial.readString();
        action(message);
        Serial.println(message);
    }
    motorMove();
}