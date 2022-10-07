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
 
// rDCmotor + lDCmotor = flywheel
const byte rDCmotor_IN1 = 32; //speed 
const byte rDCmotor_IN2 = 30; //direction
const byte lDCmotor_IN1 = 24; //speed 
const byte lDCmotor_IN2 = 26; //direction

// limit switch 都接NO (觸發時為0)
const byte topbLimswit = 7; //上後
const byte downbLimswit = 5; //下後
const byte downfLimswit = 6; //下前
const byte pusherLimswit = 4; //推桿

String message;
int times = 0;

// the status of motors
int topStepper_status;
int downDC_status;
int Pusher_status;
int flywheel_status;


// to check the position of topStepper , downDC and pusher
boolean topStepper_arrive_back = 0;
boolean downDC_arrive_back = 0;
boolean downDC_arrive_front = 0;
boolean pusher_arrive_down = 0;

// to check the task being performed
int zero_stage = 0;
int first_stage = 0;
int second_stage = 0;
int third_stage = 0;
int forth_stage = 0;

//timmer 
unsigned long start_time;

//pinMode setup
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
        delay(150);
        while(digitalRead(topbLimswit) == 0)
        {
            // forward
            StepperGo(topStepper_CW,topStepper_CLK,0); 
        }
        status = 1;
        topStepper_arrive_back = 1;
    }
}

void topStepper_task(int &status)  
{
    if (status == 1){;} //stop
    else if (status == 2) //出 
    {
        topStepper_arrive_back = 0;
        topStepper_forward();
    }
    else if (status == 3) //進
    {
        topStepper_arrive_back = 0;
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
        delay(150);
        while(digitalRead(downfLimswit) == 0)
        {
            //backward
            digitalWrite(downDC_IN1, LOW);
            digitalWrite(downDC_IN2, HIGH);
            analogWrite(downDC_ENA, 150);
        }
        status = 1;
        downDC_arrive_front = 1;
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
        delay(150);
        while(digitalRead(downbLimswit) == 0)
        {
            //forward
            digitalWrite(downDC_IN1, HIGH);
            digitalWrite(downDC_IN2, LOW);
            analogWrite(downDC_ENA, 150);
        }
        status = 1;
        downDC_arrive_back = 1;
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
        downDC_arrive_back = 0;
        downDC_arrive_front = 0;
        downDC_forward(status);
    }
    else if (status == 3) // backward
    {
        downDC_arrive_back = 0;
        downDC_arrive_front = 0;
        downDC_backward(status);
    }
}

/* Programs about the motions of flywheel */
#define speedIn 200
#define speedOut 200
void flywheel_task(int status) //flywheel
{
    if (status == 1) // stop
    {
        digitalWrite(rDCmotor_IN1, 0);
        digitalWrite(rDCmotor_IN2, LOW);
        digitalWrite(lDCmotor_IN1, 0);
        digitalWrite(lDCmotor_IN2, LOW);
    }
    else if (status == 2) // 噴
    {
        digitalWrite(rDCmotor_IN1, speedOut);
        digitalWrite(rDCmotor_IN2, LOW);
        digitalWrite(lDCmotor_IN1, speedOut);
        digitalWrite(lDCmotor_IN2, HIGH);
    }
    else if (status == 3) // 吸
    {
        digitalWrite(rDCmotor_IN1, speedIn);
        digitalWrite(rDCmotor_IN2, HIGH);
        digitalWrite(lDCmotor_IN1, speedIn);
        digitalWrite(lDCmotor_IN2, LOW);
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
        delay(150);
        while(digitalRead(pusherLimswit) == 0)
        {
            PusherUp();
        }
        status = 1; //stop
        pusher_arrive_down = 1;
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
    else if (status == 2) // down
    {
        pusher_arrive_down = 0;
        PusherDown(status);
    }
    else if (status == 3) // up
    {
        pusher_arrive_down = 0;
        PusherUp();
    }
}

/* Programs about the task */
void standard_position(int& status)
{
    if(status == 1)
    {
        Pusher_status = 2;
        topStepper_status = 3;
        downDC_status = 2;
    }
    else if(status == 1 and topStepper_arrive_back == 1 and downDC_arrive_front == 1 and pusher_arrive_down == 1)
    {
        status = 0;
        Serial.println(message);
    }
}

void takeBall_first(int& status)
{
    if(status == 1) //pusher抬高三秒
    {
        Pusher_status = 3;
        if(millis() - start_time >= 3000)
        {
            status = 2;
        }
    }
    else if (status == 2) //downDC後退到底
    {
        downDC_status = 3;
        //再兩秒pusher停
        if(millis() - start_time >= 5000)
        {
            Pusher_status = 1;
            status = 3;
        }
    }
    else if(status == 3 )
    {
        if(downDC_arrive_back == 1)
        {
            status = 4;
        }
    }
    else if (status == 4) //pusher縮到底 
    {
        Pusher_status = 2; //縮短
        if(pusher_arrive_down == 1)
        {
            status = 5;
            delay(900); //等0.9秒 此時沒有馬達再動 直接用delay
            start_time = millis();
        }
    }
    else if(status == 5)
    {
        Pusher_status = 3; //抬高
        if(millis() - start_time >= 5000)
        {
            Pusher_status = 1;
            status = 0;
            Serial.println(message);
        }
    }
}

void takeBall_rest(int& status)
{
    if(status == 1)
    {
        Pusher_status = 2; //縮短
        if(pusher_arrive_down == 1)
        {
            status = 2;
            delay(900); //等0.9秒 此時沒有馬達再動 直接用delay
            start_time = millis();
        }
    }
    else if(status == 2)
    {
        Pusher_status = 3; //抬高
        if(millis() - start_time >= 5000)
        {
            Pusher_status = 1;
            status = 0;
            Serial.println(message);
        }
    }
}

void takeBall(int& time , int& status) //取球
{
  if(time == 1 and status != 0)
  {
    takeBall_first(status);
  }
  else if((time == 2 or time ==3) and status != 0)
  {
    takeBall_rest(status);
    if(time == 3) {time = 0;}
  }
}

/* Programs about processing the msg sended from the python_server */

void task(String message)
{
    char motor_type = message[0];
    char motor_status = message[1];
    switch (motor_type)
    {
        case '2': //上
            topStepper_status = int(motor_status - '0');
            break;
        case '3': //下
            downDC_status = int(motor_status - '0');
            break;
        case '4': //推桿
            Pusher_status = int(motor_status - '0');
            break;
        case '5': //飛輪
            flywheel_status = int(motor_status - '0');  
            break; 
        case '0': //standard position
            zero_stage = 1;
            break;
        case '1': //take basketballs 
            times++;
            first_stage = 1;
            start_time = millis();
            break;
        // case '2':
        //     throwing_basketball();
        // case '3':
        //     taking_bowling();
        // case '4':
        //     relasing_bowling();
    }
}

void action()
{
    standard_position(zero_stage);
    takeBall(times,first_stage);
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
        task(message);
    }
    action();
    motorMove();
}