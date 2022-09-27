/* This will be stored in the arduino board ,and receive the msg sended from the python server */

// topStepper 
const byte topStepper_CLK = 38; // step 
const byte topStepper_CW  = 36; // direction

//downDC
const byte downDC_IN2 = 44; 
const byte downDC_IN1 = 42; 
const byte downDC_ENA = 8;

// Pusher
const byte Pusher_ENA = 9;
const byte Pusher_IN1 = 50;
const byte Pusher_IN2 = 48;
 
// rDCmotor 右馬達,  lDCmotor 左馬達 => flywheel
const byte rDCmotor_IN1 = 32; //speed 
const byte rDCmotor_IN2 = 30; //direction
const byte lDCmotor_IN1 = 24; //speed 
const byte lDCmotor_IN2 = 26; //direction

//限位開關 都接NO (觸發時為0)
const byte topbLimswit = 7; //上後
const byte downbLimswit = 5; //下後
const byte downfLimswit = 6; //下前
const byte pusherLimswit = 4; //推桿


String message;
int topStepper_status;
int downDC_status;
int Pusher_status;
int flywheel_status;

void setup()
{
    Serial.begin(57600);
    pinMode(topStepper_CLK, OUTPUT);
    pinMode(topStepper_CW,  OUTPUT);
    pinMode(downDC_IN1, OUTPUT);
    pinMode(downDC_IN2, OUTPUT);
    pinMode(downDC_ENA, OUTPUT);
    pinMode(Pusher_ENA, OUTPUT);
    pinMode(Pusher_IN1, OUTPUT);
    pinMode(Pusher_IN2, OUTPUT);
    pinMode(rDCmotor_IN1, OUTPUT);
    pinMode(rDCmotor_IN2, OUTPUT);
    pinMode(lDCmotor_IN1, OUTPUT);
    pinMode(lDCmotor_IN2, OUTPUT);
    pinMode(topbLimswit, INPUT_PULLUP); 
    pinMode(downfLimswit, INPUT_PULLUP);
    pinMode(downbLimswit, INPUT_PULLUP);
    pinMode(pusherLimswit, INPUT_PULLUP);
}

/* Programs about the motions of Stepper */

void StepperGo(const byte CW,const byte CLK,int Dir)
{
    digitalWrite(CW, Dir);
    digitalWrite(CLK, HIGH);
    delayMicroseconds(450);
    digitalWrite(CLK, LOW);
    delayMicroseconds(450);
}

/* Programs about the motions of top Stepper */
void topStepper_forward()
{
    // forward
    StepperGo(topStepper_CW,topStepper_CLK,0); 
}

void topStepper_backward(int& status)
{
    if (digitalRead(topbLimswit) == 1)
    {
        // backward
        StepperGo(topStepper_CW,topStepper_CLK,1);
    }
    else if (digitalRead(topbLimswit) == 0)
    {
        delay(100);
        while(digitalRead(topbLimswit) == 0)
        {
            // forward
            StepperGo(topStepper_CW,topStepper_CLK,0); 
        }
        status = 1;
        Serial.println("13");
    }
}

void topStepper_task(int &status)  
{
    if (status == 1){;} //stop
    else if (status == 2) //出 
    {
        topStepper_forward();
    }
    else if (status == 3) //進
    {
        topStepper_backward(status);
    }
}

/* Programs about the motions of down Stepper */

void downDC_forward(int& status)
{
    if(digitalRead(downfLimswit) == 1)
    {
        //forward
        digitalWrite(downDC_IN1, HIGH);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, 150);
    }
    else if (digitalRead(downfLimswit) == 0)
    {
        delay(100);
        while(digitalRead(downfLimswit) == 0)
        {
            //backward
            digitalWrite(downDC_IN1, LOW);
            digitalWrite(downDC_IN2, HIGH);
            analogWrite(downDC_ENA, 150);
        }
        status = 1;
        Serial.println("22");
    }
}

void downDC_backward(int& status)
{
    if(digitalRead(downbLimswit) == 1)
    {
        //backward
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, HIGH);
        analogWrite(downDC_ENA, 150);
    }
    else if (digitalRead(downbLimswit) == 0)
    {
        delay(100);
        while(digitalRead(downbLimswit) == 0)
        {
            //forward
            digitalWrite(downDC_IN1, HIGH);
            digitalWrite(downDC_IN2, LOW);
            analogWrite(downDC_ENA, 150);
        }
        status = 1;
        Serial.println("23");
    }
}

void downDC_task(int& status) 
{
    if (status == 1)
    {
        //stop
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, 0);
    } 
    else if (status == 2) // forward
    {
        downDC_forward(status);
    }
    else if (status == 3) // backward
    {
        downDC_backward(status);
    }
}

/* Programs about the motions of flywheel */

void flywheel_task(int status) //flywheel
{
    if (status == 1) // stop
    {
        digitalWrite(rDCmotor_IN1, 0);
        digitalWrite(rDCmotor_IN2, LOW);
        digitalWrite(lDCmotor_IN1, 0);
        digitalWrite(lDCmotor_IN2, LOW);
    }
    else if (status == 2) // shoot
    {
        digitalWrite(rDCmotor_IN1, 1);
        digitalWrite(rDCmotor_IN2, LOW);
        digitalWrite(lDCmotor_IN1, 1);
        digitalWrite(lDCmotor_IN2, HIGH);
    }
}

/* Programs about the motions of Pusher */

void PusherUp()
{
    digitalWrite(Pusher_IN1, HIGH);
    digitalWrite(Pusher_IN2, LOW);
    analogWrite(Pusher_ENA, 150);
}

void PusherDown(int& status)
{
    if(digitalRead(pusherLimswit) == 1)
    {
        digitalWrite(Pusher_IN1, LOW);
        digitalWrite(Pusher_IN2, HIGH);
        analogWrite(Pusher_ENA, 150);
    }
    else if(digitalRead(pusherLimswit) == 0)
    {
        delay(100);
        while(digitalRead(pusherLimswit) == 0)
        {
            PusherUp();
        }
        status = 1; //stop
        Serial.println("32");
    }
}

void PusherStop()
{
    digitalWrite(Pusher_IN1, LOW);
    digitalWrite(Pusher_IN2, LOW);
    analogWrite(Pusher_ENA, 0);
}

void Pusher_task(int& status) 
{
    if (status == 1) // stop
    {
        PusherStop();
    }
    else if (status == 2) //縮短
    {
        PusherDown(status);
    }
    else if (status == 3) //伸長
    {
        PusherUp();
    }
}

/* Programs about processing the msg sended from the python_server */

// topStepper 11, 12, 13退 
// downDC 21, 22, 23退
// Pusher 31, 32, 33伸
// flywheel 41, 42, 

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
            downDC_status = int(motor_status - '0');
            break;
        case '3': //推桿
            Pusher_status = int(motor_status - '0');
            break;
        case '4': //飛輪
            flywheel_status = int(motor_status - '0');  
            break; 
    }
}

void motorMove()
{
    topStepper_task(topStepper_status);
    downDC_task(downDC_status);
    Pusher_task(Pusher_status);
    flywheel_task(flywheel_status); 
}

void loop()
{
    if (Serial.available() > 0)
    {
        message = Serial.readString();
        action(message);
    }
    motorMove();
}
