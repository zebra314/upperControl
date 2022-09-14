/* This will be stored in the arduino board ,and receive the msg sended from the python server */

// topStepper ,  downStepper
const byte topStepper_CLK = 44; // step 
const byte topStepper_CW  = 26; // direction
const byte downStepper_CLK = 31; // step
const byte downStepper_CW  = 46; // direction

// Pusher
const byte Pusher_ENA = 11;
const byte Pusher_IN1 = 9;
const byte Pusher_IN2 = 12;
 
// rDCmotor 右馬達,  lDCmotor 左馬達 => flywheel
const byte rDCmotor_IN1 = 3; //speed 
const byte rDCmotor_IN2 = 24; //direction
const byte lDCmotor_IN1 = 2; //speed 
const byte lDCmotor_IN2 = 22; //direction

//限位開關 都接NO (觸發時為0)
const byte topbLimswit = 7; //上後
const byte downbLimswit = 5; //下後
const byte pusherLimswit = 4; //推桿

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
    pinMode(topbLimswit, INPUT_PULLUP); 
    pinMode(downbLimswit, INPUT_PULLUP);
    pinMode(pusherLimswit, INPUT_PULLUP);
}

void StepperGo(const byte CW,const byte CLK,int Dir)
{
    digitalWrite(CW, Dir);
    digitalWrite(CLK, HIGH);
    delayMicroseconds(500);
    digitalWrite(CLK, LOW);
    delayMicroseconds(500);
}

/* Programs about the motions of top Stepper */
void topStepper_task(int &status)  
{
    if (status == 1){;} //stop
    else if (status == 2) //出 
    {
        StepperGo(topStepper_CW,topStepper_CLK,0); //出
    }
    else if (status == 3) //進
    {
        if(digitalRead(topbLimswit) == 0)
        {
            delay(150);
            while(digitalRead(topbLimswit) == 0)
            {
                StepperGo(topStepper_CW,topStepper_CLK,0); //出
            }
            status = 1; 
        }
        else
        {
            StepperGo(topStepper_CW,topStepper_CLK,1); //進
        }
    }
}

/* Programs about the motions of down Stepper */
void downStepper_task(int& status) 
{
    if (status == 1){;} // stop
    else if (status == 2) //出
    {
        //出
        StepperGo(downStepper_CW,downStepper_CLK,1);
    }
    else if (status == 3) //進
    {
        if(digitalRead(downbLimswit) == 0)
        {
            delay(150);
            while(digitalRead(downbLimswit) == 0)
            {
                //出
                StepperGo(downStepper_CW,downStepper_CLK,1);
            }
            status = 1; //stop
        }
        else
        {
            //進
            StepperGo(downStepper_CW,downStepper_CLK,0);
        }
    }
}

/* Programs about the motions of flywheel */
#define speedIn 200
#define speedOut 200
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
        analogWrite(rDCmotor_IN1, speedIn);
        digitalWrite(rDCmotor_IN2, HIGH);
        analogWrite(lDCmotor_IN1, speedIn);
        digitalWrite(lDCmotor_IN2, LOW);
    }
    else if (status == 3) // 吸
    {
        analogWrite(rDCmotor_IN1, speedOut);
        digitalWrite(rDCmotor_IN2, LOW);
        analogWrite(lDCmotor_IN1, speedOut);
        digitalWrite(lDCmotor_IN2, HIGH);
    }
}

/* Programs about the motions of Pusher */
void PusherUp()
{
    digitalWrite(Pusher_IN1, HIGH);
    digitalWrite(Pusher_IN2, LOW);
    analogWrite(Pusher_ENA, 100);
}

void PusherDown()
{
    digitalWrite(Pusher_IN1, LOW);
    digitalWrite(Pusher_IN2, HIGH);
    analogWrite(Pusher_ENA, 100);
}

void PusherStop()
{
    digitalWrite(Pusher_IN1, LOW);
    digitalWrite(Pusher_IN2, LOW);
    analogWrite(Pusher_ENA, 0);
}

void takeBall() //取球
{
    while(digitalRead(pusherLimswit) == 1)
    {
        PusherDown(); 
    }
    PusherStop();
    delay(100);
    for(int i = 0 ; i<3 ; i++)
    {
        PusherUp(); //伸長
        delay(3000);
        PusherStop();
        delay(300);
        while(digitalRead(pusherLimswit) == 1)
        {
            PusherDown(); 
        }
        PusherStop();
        delay(1100);
    }
    PusherUp(); 
    delay(4000);
    PusherStop();
}

void Pusher_task(int& status) 
{
    if (status == 1) // stop
    {
        PusherStop();
    }
    else if (status == 2) //縮短
    {
        if(digitalRead(pusherLimswit) == 0)
        {
            delay(150);
            while(digitalRead(pusherLimswit) == 0)
            {
                PusherUp();
            }
            status = 1; //stop
        }
        else
        {
            PusherDown();
        }
    }
    else if (status == 3) //伸長
    {
        PusherUp();
    }
}

/* Programs about the task */
void StandardPosi()
{
    Pusher_status = 1;
    topStepper_status = 3;
}

void releaseBall()
{
    flywheel_status = 2;
    topStepper_status = 2;
}

/* Programs about processing the msg sended from the python_server */

// topStepper 11, 12, 13退 
// downStepper 21, 22, 23退
// Pusher 31, 32, 33伸
// flywheel 41, 42, 43吸

void action(String message)
{
    char motor_type = message[0];
    char motor_status = message[1];
    switch (motor_type)
    {
        case '1': //上
            topStepper_status = int(motor_status - '0');
            break;
        case '2': //下
            downStepper_status = int(motor_status - '0');
            break;
        case '3': //推桿
            Pusher_status = int(motor_status - '0');
            break;
        case '4': //飛輪
            flywheel_status = int(motor_status - '0');  
            break; 
        case '5': //standard position
            StandardPosi();
            break;
        case '6': //take basketballs 
            takeBall();
            break;
        case '7':
            releaseBall();
            break;

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
        message = Serial.readString();
        Serial.println(message);
        action(message);
    }
    motorMove();
}
