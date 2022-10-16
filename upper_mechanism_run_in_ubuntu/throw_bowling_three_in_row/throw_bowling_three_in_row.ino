/* This will be stored in the arduino board ,and receive the msg sended lyrom the python server */

// topStepper
const byte topStepper_CLK = 38; // step 
const byte topStepper_CW  = 36; // direction

//downDC
const byte downDC_IN1 = 44; 
const byte downDC_IN2 = 42; 
const byte downDC_ENA = 8;

// Pusher
const byte Pusher_ENA = 9;
const byte Pusher_IN1 = 50; 
const byte Pusher_IN2 = 48;
 
// rDCmotor + lDCmotor = flywheel
const byte rDCmotor_IN1 = 32; // speed 
const byte rDCmotor_IN2 = 30; // direction
const byte rDCmotor_IN3 = 51; // brake
const byte lDCmotor_IN1 = 24; // speed 
const byte lDCmotor_IN2 = 26; // direction

// limit switch 接NO 
const byte topbLimswit = 4; //上後
const byte downbLimswit = 6; //下後
const byte downfLimswit = 5; //下前
const byte pusherLimswit = 7; //推桿

// fly wheel switch 接NO  
const byte flySwitch = 22; // the Relay of fly wheel

// store the msg sended from the server
String message;

// By changing this status , we can controll the motors manually
int topStepper_status;
int downDC_status;
int Pusher_status;
int flywheel_status;

// To record the current stage of the task in the script 
int takeBasket_status = 0;
int throwBasket_status = 0; 
int throwBowling_status = 0;

// To record the amount of ball we have taken or thrown
int takeBasket_times = 0;
int takeBowling_times = 0;
int throwBasket_times = 0;
int throwBowling_times = 0;

// To record the time by using millis()
int start_time = 0;

// To setup the pin mode
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
    pinMode(flySwitch, OUTPUT);
}

/* Programs about the motions of Stepper */
//  1  forward
// -1  backward
//  0  stop
void topStepper_Go(int dir) 
{
    if(dir == 1)
    {
        // forward
        digitalWrite(topStepper_CW, LOW);
        digitalWrite(topStepper_CLK, HIGH);
        delayMicroseconds(450);
        digitalWrite(topStepper_CLK, LOW);
        delayMicroseconds(450);
    }
    else if(dir == -1)
    {
        // backward
        digitalWrite(topStepper_CW, HIGH);
        digitalWrite(topStepper_CLK, HIGH);
        delayMicroseconds(450);
        digitalWrite(topStepper_CLK, LOW);
        delayMicroseconds(450);
    }
    else if(dir == 0)
    {
        ;
    }
}
void topStepper_forward()
{
    // forward
    topStepper_Go(1); 
}
void topStepper_backward(int& status)
{
    if (digitalRead(topbLimswit) == 1)
    {
        // backward
        topStepper_Go(-1);
    }
    else if (digitalRead(topbLimswit) == 0)
    {
        delay(100);
        while(digitalRead(topbLimswit) == 0)
        {
            // forward
            topStepper_Go(1); 
        }
        status = 1;
    }
}
void topStepper_task(int& status)  
{
    if (status == 1){;} 
    else if (status == 2) 
    {
        topStepper_forward();
    }
    else if (status == 3) 
    {
        topStepper_backward(status);
    }
}

/* Programs about the motions of down Stepper , for script */
// 1 forward
// -1 backward
// 0 stop
void downDC_Go(int dir , int speed)
{
    if(dir == 1)
    { 
        //forward
        digitalWrite(downDC_IN1, HIGH);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, speed);
    }
    else if(dir == -1) 
    {
        //backward
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, HIGH);
        analogWrite(downDC_ENA, speed);
    }
    else if(dir == 0)
    {
        //stop
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, 0);
    }
}
void downDC_forward(int& status ,int speed)
{
    if(digitalRead(downfLimswit) == 1)
    { 
        // forward
        // normal = 150
        // fast = 250
        downDC_Go(1,speed);
    }
    else if (digitalRead(downfLimswit) == 0)
    {
        // stop
        downDC_Go(0,0);
        delay(100);
        while(digitalRead(downfLimswit) == 0)
        {
            // backward
            downDC_Go(-1,220);
        }
        status = 1;
    }
}

void downDC_backward(int& status)
{
    if(digitalRead(downbLimswit) == 1)
    {
        // backward
        // normal = 150
        // fast = 200
        downDC_Go(-1,150);
    }
    else if (digitalRead(downbLimswit) == 0)
    {
        // stop
        downDC_Go(0,0);
        delay(100);
        while(digitalRead(downbLimswit) == 0)
        {
            // forward
            downDC_Go(1,220);
        }
        status = 1;
    }
}
void downDC_task(int& status) 
{
    if (status == 1)
    {
        // stop
        downDC_Go(0,0);
    } 
    else if (status == 2) // forward
    {
        downDC_forward(status,150);
    }
    else if (status == 3) // backward
    {
        downDC_backward(status);
    }
}

/* Programs about the motions of flywheel */
#define speedOut 200
void flywheel_task(int status) //flywheel
{
    if (status == 1) 
    {   
        // stop
        digitalWrite(flySwitch, LOW);
        digitalWrite(rDCmotor_IN1, 0);
        digitalWrite(rDCmotor_IN2, LOW);
        digitalWrite(lDCmotor_IN1, 0);
        digitalWrite(lDCmotor_IN2, LOW);
    }
    else if (status == 2) 
    {
        // out
        digitalWrite(flySwitch, HIGH);
        digitalWrite(rDCmotor_IN1, speedOut);
        digitalWrite(rDCmotor_IN2, LOW);
        digitalWrite(lDCmotor_IN1, speedOut);
        digitalWrite(lDCmotor_IN2, HIGH);
    }
}

/* Programs about the motions of Pusher , for script*/
// 1 up
// -1 down
// 0 stop
void Pusher_Go(int dir , int speed)
{
    if(dir == 1)
    {
        digitalWrite(Pusher_IN1, HIGH);
        digitalWrite(Pusher_IN2, LOW);
        analogWrite(Pusher_ENA, speed);
    }
    else if(dir == -1)
    {
        digitalWrite(Pusher_IN1, LOW);
        digitalWrite(Pusher_IN2, HIGH);
        analogWrite(Pusher_ENA, speed);
    }
    else if(dir == 0)
    {
        digitalWrite(Pusher_IN1, LOW);
        digitalWrite(Pusher_IN2, LOW);
        analogWrite(Pusher_ENA, 0);
    }
}

void PusherDown(int& status)
{
    if(digitalRead(pusherLimswit) == 1)
    {
        Pusher_Go(-1,150);
    }
    else if(digitalRead(pusherLimswit) == 0)
    {
        Pusher_Go(0,0);
        delay(100);
        while(digitalRead(pusherLimswit) == 0)
        {
            Pusher_Go(1,250);
        }
        status = 1; 
    }
}
void PusherDown_slow(int& status)
{
    if(digitalRead(pusherLimswit) == 1)
    {
        Pusher_Go(-1,100);
    }
    else if(digitalRead(pusherLimswit) == 0)
    {
       Pusher_Go(0,0);
        delay(100);
        while(digitalRead(pusherLimswit) == 0)
        {
            Pusher_Go(1,250);
        }
        status = 1; 
    }
}
void PusherDown_fast(int& status)
{
    if(digitalRead(pusherLimswit) == 1)
    {
        Pusher_Go(-1,250);
    }
    else if(digitalRead(pusherLimswit) == 0)
    {
        Pusher_Go(0,0);
        delay(100);
        while(digitalRead(pusherLimswit) == 0)
        {
            Pusher_Go(1,250);;
        }
        status = 1; //stop
    }
}

void Pusher_task(int& status) 
{
    if (status == 1)
    { 
        // stop
        Pusher_Go(0,0);
    }
    else if (status == 2) 
    {
        // down
        PusherDown(status);
    }
    else if (status == 3) 
    {
        // up
        Pusher_Go(1,150);
    }
}

/* Programs about the task */
//standard position
void StandardPosi()
{
    Serial.println(message);
    //downDC forward
    while(digitalRead(downfLimswit) == 1)
    {
        while(digitalRead(downfLimswit) == 1)
        {
            //forward
            downDC_Go(1,150);
        }
    }
    downDC_Go(0,0);
    delay(150);
    while(digitalRead(downfLimswit) == 0)
    {
        //backward
        downDC_Go(-1,200);
    }
    downDC_Go(0,0);

    //topStepper backward
    while(digitalRead(topbLimswit) == 1)
    {
        // backward
        topStepper_Go(-1);
    }
    delay(100);
    while(digitalRead(topbLimswit) == 0)
    {
        // forward
        topStepper_Go(1);
    }

    //pusher down
    while(digitalRead(pusherLimswit) == 1)
    {
        Pusher_Go(-1,150);
    }
    delay(100);
    while(digitalRead(pusherLimswit) == 0)
    {
        //up
        Pusher_Go(1,200);
    }
    Pusher_Go(0,0);
}

//take basketball (take one ball at a time)
void takeBasket_three(int& time)
{
    if(time == 1)
    {
        Pusher_Go(1,150); //up
        delay(5750);
        Pusher_Go(0,0); //stop
        while(digitalRead(downbLimswit) == 1)
        {
            while(digitalRead(downbLimswit) == 1)
            {
                //backward
                downDC_Go(-1,200);
            }
        }
        downDC_Go(0,0);
        delay(150);
        while(digitalRead(downbLimswit) == 0)
        {
            //forward
            downDC_Go(1,200);
        }
        downDC_Go(0,0);
        while(digitalRead(pusherLimswit) == 1)
        {
            // down
            Pusher_Go(-1,150);
        }
        Pusher_Go(0,0);
        delay(2000);
        Pusher_Go(1,150); //up
        delay(3500);
        Pusher_Go(0,0);
        Serial.println(message);
    } 
    else if(time == 2)
    {
        while(digitalRead(pusherLimswit) == 1)
        {
            Pusher_Go(-1,150);
        }
        Pusher_Go(0,0);
        delay(1500);
        Pusher_Go(1,150); //up
        delay(4500);
        Pusher_Go(0,0);
        Serial.println(message);
    }
    else if(time ==3)
    {
        while(digitalRead(pusherLimswit) == 1)
        {
            Pusher_Go(-1,150);
        }
        Pusher_Go(0,0);
        delay(300);
        Pusher_Go(1,150); //up
        delay(4500);
        Pusher_Go(0,0);
        Serial.println(message);
        time = 0;

        //prepare for throwing the first basket
        delay(5000);
        while(digitalRead(pusherLimswit) == 1)
        {
            Pusher_Go(-1,150);
        }
        Pusher_Go(0,0);
        delay(100);
        Pusher_Go(1,90);; //up
        delay(1300); //水平
        Pusher_Go(0,0);
    }
}

void throwing_basketball_three(int& time , int& status)
{
    int duration = millis()-start_time ;
    int MOTstatus = 0;
    if(time == 1)
    {
        if(status == 1)
        {
            status = 2;
            start_time = millis();
        }
        else if(status == 2 and duration < 3000 ) 
        {
            flywheel_task(2);
        }
        else if(status == 2 and duration > 3000 )
        {
            status = 3;
            start_time = millis();
            downDC_status = 2;
        }
        else if(status == 3 and duration < 4000 )
        {
            // downDC , topStepper , flywheel be activated at the same time 
            // It is too dangerous to stop the motor only by time
            // we must use the _task() function , which contain the limit switch check
            downDC_task(downDC_status); 
            topStepper_Go(1);
            flywheel_task(2);
        }
        else if(status == 3 and duration > 4000)
        {   
            downDC_status = 1;
            downDC_Go(0,0); 
            topStepper_Go(0);
            flywheel_task(1);
            Serial.println(message);
            status = 0;
        }
    }
    else if(time == 2)
    {
        if(status == 1)
        {
            MOTstatus = 3;
            Pusher_task(MOTstatus);
            Pusher_Go(1,150);
            delay(6000); 
            Pusher_Go(0,0);
            delay(200);
            Pusher_Go(-1,150);
            // delay(1440);
            delay(800);
            Pusher_Go(0,0);
            status = 2;
            start_time = millis();
        }
        else if(status == 2 and duration < 4000 ) 
        {
            flywheel_task(2);
        }
        else if(status == 2 and duration > 4000 )
        {
            status = 3;
            start_time = millis();
            downDC_status = 2;
        }
        else if(status == 3 and duration < 3750 )
        {
            //downDC , topStepper , flywheel be activated at the same time 
            MOTstatus = 2;
            downDC_forward(MOTstatus,250); 
            topStepper_task(MOTstatus);
            downDC_task(MOTstatus); 
            topStepper_Go(1);
            flywheel_task(2);
        }
        else if(status == 3 and duration > 3750 )
        {
            downDC_status = 1;
            downDC_Go(1,150); 
            flywheel_task(1);
            Serial.println(message);
            status = 0;
        }
    }
    else if(time == 3)
    {
        if(status ==1)
        {
            status = 2;
            start_time = millis();
        }
        if(status == 2 and duration < 3000)
        {
            flywheel_task(2); 
        }
        else if(status == 2 and duration > 3000)
        {
            status = 3;
            start_time = millis();
            downDC_status = 2;
        }
        else if(status == 3 and duration < 3000 ) 
        {
            // downDC , topStepper , flywheel be activated at the same time 
            downDC_task(downDC_status); 
            topStepper_Go(1);
            flywheel_task(2);
            if(digitalRead(downfLimswit) == 0)
            {
                while(digitalRead(downfLimswit) == 0)
                {
                    downDC_Go(-1,220);
                }
                status = 0;
                int STstatus = 1;
                topStepper_task(STstatus);
                int DCstatus = 1;
                downDC_task(DCstatus); 
                flywheel_task(1);
                status = 0;
                Serial.println(message);
            }
        }
        else if(status == 3 and duration > 3000)
        {
            topStepper_Go(0);
            downDC_status = 1;
            downDC_Go(0,0);
            flywheel_task(1);
            status = 0;
            time = 0;
            Serial.println(message);
        }    
    }
}

//take bowlingball (take one ball at a time)
void takeBowling_three(int& time) 
{
    if(time == 1)
    {
        Pusher_Go(1,250); //up
        delay(5000);
        Pusher_Go(0,0); //stop
        while(digitalRead(downbLimswit) == 1)
        {
            while(digitalRead(downbLimswit) == 1)
            {
                //backward
                downDC_Go(-1,150);
            }
        }
        downDC_Go(0,0);
        delay(150);
        while(digitalRead(downbLimswit) == 0)
        {
            downDC_Go(1,150);
        }
        downDC_Go(0,0);
        while(digitalRead(pusherLimswit) == 1)
        {
            Pusher_Go(-1,150); //down
        }
        Pusher_Go(0,0);
        delay(2500);
        Pusher_Go(1,90); //up
        delay(4500);
        Pusher_Go(0,0);
        Serial.println(message);
    } 
    else if(time == 2)
    {
        while(digitalRead(pusherLimswit) == 1)
        {
            //down
            Pusher_Go(-1,150);
        }
        Pusher_Go(0,0);
        delay(2500);
        Pusher_Go(1,90);; //up
        delay(4500);
        Pusher_Go(0,0);
        Serial.println(message);
    }
    else if(time ==3)
    {
        while(digitalRead(pusherLimswit) == 1)
        {
            //down
            Pusher_Go(-1,150);
        }
        Pusher_Go(0,0);
        delay(300);
        Pusher_Go(1,90); //up
        delay(4500);
        Pusher_Go(0,0);
        Serial.println(message);
        time = 0;

        //to make camera able to face the card board
        delay(4000);
        while(digitalRead(pusherLimswit) == 1)
        {
            //down
            Pusher_Go(-1,150);
        }
        Pusher_Go(0,0);
        Pusher_Go(1,100); //up
        delay(1300); //水平
        Pusher_Go(0,0);
    }
}

//take bowlingball (take one ball at a time)
void takeBowling_once() //
{
    while(digitalRead(pusherLimswit) == 1)
    {
        Pusher_Go(-1,150); //down
        Pusher_Go(0,0);
        delay(100);
        Pusher_Go(1,90); //up
        delay(1300);  //水平
        Pusher_Go(0,0);
    }
}

//throw bowling ball (throw one ball at a time)
void releasing_bowling(int& time , int& status)
{
    int duration = millis() - start_time;
    int Motstatus = 0;
    if(time == 1)
    {
        if(status == 1)
        {
            Pusher_Go(1,150);            
            Motstatus = 2;
            downDC_forward(Motstatus,150); //forward
            delay(2000);
            Pusher_Go(0,0);
            Motstatus = 1;
            downDC_task(Motstatus); 
            flywheel_task(2);
            delay(400);
            start_time = millis() ;
            while(digitalRead(pusherLimswit) == 1)
            { 
                duration = millis() - start_time;
                PusherDown_slow(Pusher_status); //down
                if( duration > 1600)
                {
                    flywheel_task(1);
                }
                else
                {
                    flywheel_task(2);
                    topStepper_Go(1);
                }
            }
            flywheel_task(1);
            Pusher_Go(0,0);
            delay(150);
            Pusher_Go(1,250);; //down
            delay(500);
            Pusher_Go(0,0);
            status = 2;
            start_time = millis();
        }
        else if(status == 2 and duration < 2000 )
        {
            Motstatus = 3;
            topStepper_task(Motstatus);
        }
        else if(status == 2 and duration > 2000 )
        {
            topStepper_task(Motstatus);
            Serial.println(message);
            status = 0;
        }
    }
    else if(time == 2 )
    {
        int duration = millis() - start_time ;
        if(status == 1 )
        {
            Pusher_Go(1,150);
            delay(1500);
            Pusher_Go(0,0);
            flywheel_task(2);
            delay(400);
            start_time = millis() ;
            while(digitalRead(pusherLimswit) == 1)
            {
                duration = millis() - start_time;
                PusherDown_slow(Pusher_status); //down
                if( duration > 1350)
                {
                    flywheel_task(1);
                }
                else
                {
                    flywheel_task(2);
                }
            }
            flywheel_task(1);
            Pusher_Go(0,0);
            delay(100);
            Pusher_Go(1,250);; //down
            Motstatus = 2 ;
            downDC_task(Motstatus); 
            delay(1200);
            Pusher_Go(0,0);
            status = 2;
            Motstatus = 1 ;
            downDC_task(Motstatus);
            start_time = millis();
        }
        else if(status == 2 and duration < 3500)
        {
            Motstatus = 2;
            topStepper_task(Motstatus);
        }
        else if(status == 2 and duration > 3500)
        {
            Serial.println(message);
            status = 0; 
        }
    }
    else if(time == 3)
    {
        if(status == 1 )
        {
            Pusher_Go(1,150);
            delay(1500);
            Pusher_Go(0,0);
            delay(400);
            while(digitalRead(pusherLimswit) == 1)
            {
                flywheel_task(2);
                PusherDown_slow(Pusher_status); //down
            }
            flywheel_task(1);
            Pusher_Go(0,0);
            delay(200);
            while(digitalRead(pusherLimswit) == 0)
            {
                Pusher_Go(1,90);; //down
            }
            Pusher_Go(0,0);
            StandardPosi();
            Pusher_Go(0,0);
            Serial.println(message);
            status = 2;
            time = 0;
        }
    }
}

/* Programs about processing the msg sended from the python_server */
void action(String message)
{
    char motor_type = message[0];
    char motor_status = message[1];
    switch (motor_type)
    {
        case '0': 
            StandardPosi();
            break;
        case '1': 
            takeBasket_times++;
            takeBasket_three(takeBasket_times);
            break;
        case '2': 
            throwBasket_times ++;
            throwBasket_status = 1;
            start_time = millis();
            break;
        case '3': 
            takeBowling_times ++;
            takeBowling_three(takeBowling_times);
            break;
        case '4':
            throwBowling_times++;
            throwBowling_status = 1;
            // start_time = millis();        
        case '5': //上
            topStepper_status = int(motor_status - '0');
            break;
        case '6': //下
            downDC_status = int(motor_status - '0');
            break;
        case '7': //推桿
            Pusher_status = int(motor_status - '0');
            break;
        case '8': //飛輪
            flywheel_status = int(motor_status - '0');  
            break;                 
    }
}

// run every cycle in the void loop
void motorMove()
{
    /* control the motor by changing the status */
    topStepper_task(topStepper_status);
    downDC_task(downDC_status);
    Pusher_task(Pusher_status);
    flywheel_task(flywheel_status); 

    /* more than two motor need to be activated at the same , therefore we put the function here */

    // takeBasket_Two_One_multi(takeBasket_times,takeBasket_status); 

    throwing_basketball_three(throwBasket_times,throwBasket_status);
    // throwing_basketball_three(throwBasket_times,throwBasket_status);

    releasing_bowling(throwBowling_times, throwBowling_status);
    // releasing_bowling_three(throwBowling_times, throwBowling_status);
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

/*
//take basketball (take one ball at a time)
void takeBasket_Two_One(int& time) 
{
    if(time == 1)
    {
        Pusher_Go(1,150); //up
        delay(5750);
        Pusher_Go(0,0); //stop
        while(digitalRead(downbLimswit) == 1)
        {
            while(digitalRead(downbLimswit) == 1)
            {
                //backward
                digitalWrite(downDC_IN1, LOW);
                digitalWrite(downDC_IN2, HIGH);
                analogWrite(downDC_ENA, 150);
            }
        }
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, 0);
        delay(250);
        while(digitalRead(downbLimswit) == 0)
        {
            //forward
            digitalWrite(downDC_IN1, HIGH);
            digitalWrite(downDC_IN2, LOW);
            analogWrite(downDC_ENA, 150);
        }
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, 0);
        while(digitalRead(pusherLimswit) == 1)
        {
            PusherDown(Pusher_status); //down
        }
        Pusher_Go(0,0);
        delay(2000);
        Pusher_Go(1,150); //up
        delay(3500);
        Pusher_Go(0,0);
        Serial.println(message);
    } 
    else if(time == 2)
    {
        while(digitalRead(pusherLimswit) == 1)
        {
            PusherDown(Pusher_status); //down
        }
        Pusher_Go(0,0);
        delay(1500);
        Pusher_Go(1,150); //up
        delay(4500);
        Pusher_Go(0,0);
        Serial.println(message);

        // let the camera to face the text board
        delay(3000);
        Pusher_Go(1,150);
        delay(3000);
        Pusher_Go(0,0);
    }
    else if(time ==3)
    {
        Pusher_Go(1,150); //up
        delay(3000);
        Pusher_Go(0,0); //stop
        while(digitalRead(pusherLimswit) == 1)
        {
            PusherDown(Pusher_status); //down
        }
        Pusher_Go(0,0);
        delay(300);
        Pusher_Go(1,150); //up
        delay(4500);
        Pusher_Go(0,0);
        Serial.println(message);
        time = 0;
    }
}

//throw basketball (throw one ball at a time)
void throwing_basketball_Two_One(int& time , int& status)
{
    int duration = millis()-start_time ;
    if(time == 1)
    {
        if(status == 1)
        {
            Pusher_Go(1,150);
            delay(2000); 
            Pusher_Go(0,0);
            delay(200);
            PusherDown(Pusher_status);
            // delay(1440);
            delay(800);
            Pusher_Go(0,0);
            status = 2;
            start_time = millis();
        }
        else if(status == 2 and duration < 4000 ) 
        {
            flywheel_task(2);
        }
        else if(status == 2 and duration > 4000 )
        {
            status = 3;
            start_time = millis();
        }
        else if(status == 3 and duration < 6900 )
        {
            //downDC , topStepper , flywheel be activated at the same time 
            int Motstatus = 2;
            downDC_forward_fast(Motstatus); 
            topStepper_task(Motstatus);
            flywheel_task(2);
        }
        else if(status == 3 and duration > 6900 )
        {
            int DCstatus = 1;
            downDC_task(DCstatus); 
            flywheel_task(1);
            Serial.println(message);
            status = 0;
        }
    }
    else if(time == 2)
    {
        if(status ==1)
        {
            Pusher_Go(1,150);
            delay(1000); 
            Pusher_Go(0,0);
            status = 2;
        }
        if(status == 2 and duration < 3000)
        {
            flywheel_task(2); 
        }
        else if(status == 2 and duration > 3000)
        {
            status = 3;
            start_time = millis();
        }
        else if(status == 3 and duration < 3250 ) 
        {
            //downDC , topStepper , flywheel be activated at the same time 
            int DCstatus = 2;
            downDC_task(DCstatus); 
            int STstatus = 2;
            topStepper_task(STstatus);
            flywheel_task(2);
        }
        else if(status == 3 and duration > 3250)
        {
            int STstatus = 1;
            topStepper_task(STstatus);
            int DCstatus = 1;
            downDC_task(DCstatus); 
            flywheel_task(1);
            status = 0;
            topStepper_status = 3;
            downDC_status = 3;
            Serial.println(message);
        }    
    }
    //中間有一次取球
    else if(time == 3)
    {
        if(status == 1)
        {
            Pusher_Go(1,150);
            delay(5000); 
            Pusher_Go(0,0);
            // delay(200);
            // PusherDown(Pusher_status);
            // delay(1440);
            // Pusher_Go(0,0);
            status = 2;
            start_time = millis();
        }
        else if(status == 2 and duration < 4000 ) 
        {
            flywheel_task(2);
            int DCstatus = 2;
            downDC_task(DCstatus); 
            int STstatus = 2;
            topStepper_task(STstatus);
        }
        else if(status == 2 and duration > 4000 )
        {
            status = 3;
            start_time = millis();
        }
        else if(status == 3 and duration < 7000 ) //從最底到最高
        {
            //downDC , topStepper , flywheel be activated at the same time 
            int DCstatus = 2;
            downDC_task(DCstatus); 
            int STstatus = 2;
            topStepper_task(STstatus);
            flywheel_task(2);
        }
        else if(status == 3 and duration > 7000)
        {
            int DCstatus = 1;
            downDC_task(DCstatus); 
            flywheel_task(1);
            status = 0;
            time = 0;
            // this function will send the msg back to the python server , 
            // therefore no needed of the serial.println()
            delay(1000);
            StandardPosi();
        }
    }
}

//throw basketball (throw one ball at a time)
//always fail
void throwing_basketball_three(int& time , int& status)
{
    int duration = millis()-start_time ;
    if(time == 1)
    {
        if(status == 1)
        {
            Pusher_Go(1,150);
            delay(5000); 
            Pusher_Go(0,0);
            // delay(200);
            // PusherDown(Pusher_status);
            // delay(1440);
            // Pusher_Go(0,0);
            status = 2;
            start_time = millis();
        }
        else if(status == 2 and duration < 4000 ) 
        {
            flywheel_task(2);
        }
        else if(status == 2 and duration > 4000 )
        {
            status = 3;
            start_time = millis();
        }
        else if(status == 3 and duration < 3000 ) //秒數待訂
        {
            //downDC , topStepper , flywheel be activated at the same time 
            int DCstatus = 2;
            downDC_task(DCstatus); 
            int STstatus = 2;
            topStepper_task(STstatus);
            flywheel_task(2);
        }
        else if(status == 3 and duration > 3000)
        {   
            int DCstatus = 1;
            downDC_task(DCstatus); 
            flywheel_task(1);
            Serial.println(message);
            status = 0;
        }
    }
    else if(time == 2 )
    {
        if(status == 1 and duration < 3000)
        {
            flywheel_task(2); 
        }
        else if(status == 1 and duration > 3000)
        {
            status = 2;
            start_time = millis();
        }
        else if(status == 2 and duration < 5300 ) 
        {
            //downDC , topStepper , flywheel be activated at the same time 
            int DCstatus = 2;
            downDC_task(DCstatus); 
            int STstatus = 2;
            topStepper_task(STstatus);
            flywheel_task(2);
        }
        else if(status == 2 and duration > 5300)
        {
            int STstatus = 1;
            topStepper_task(STstatus);
            int DCstatus = 1;
            downDC_task(DCstatus); 
            flywheel_task(1);
            Serial.println(message);
            status = 0;
        }
    }
    else if(time == 3)
    {
        if(status == 1 and duration < 3000)
        {
            flywheel_task(2); 
        }
        else if(status == 1 and duration > 3000)
        {
            status = 2;
            start_time = millis();
        }
        else if(status == 2 and duration < 4500 ) //秒數待訂
        {
            //downDC , topStepper , flywheel be activated at the same time 
            int DCstatus = 2;
            downDC_task(DCstatus); 
            int STstatus = 2;
            topStepper_task(STstatus);
            flywheel_task(2);
        }
        else if(status == 2 and duration > 4500)
        {
            int STstatus = 1;
            topStepper_task(STstatus);
            int DCstatus = 1;
            downDC_task(DCstatus); 
            flywheel_task(1);
            Serial.println(message);
            status = 0;

            //this function will send the msg back to the python server , 
            //therefore no needed of the serial.println()
            delay(1000);
            StandardPosi();  
        }
    }
}

void pre_taking_bowling()
{
    Pusher_Go(1,250);; //up
    delay(5000);
    Pusher_Go(0,0); //stop
    while(digitalRead(downbLimswit) == 1)
        {
            while(digitalRead(downbLimswit) == 1)
            {
               //backward
               digitalWrite(downDC_IN1, LOW);
               digitalWrite(downDC_IN2, HIGH);
               analogWrite(downDC_ENA, 200);
            }
        }
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, 0);
        delay(100);
        while(digitalRead(downbLimswit) == 0)
        {
            //forward
            digitalWrite(downDC_IN1, HIGH);
            digitalWrite(downDC_IN2, LOW);
            analogWrite(downDC_ENA, 200);
        }
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, 0);
    Serial.println(message);
}

void horizon_pusher()
{
    while(digitalRead(pusherLimswit) == 1)
    {
        // pusher down
        digitalWrite(Pusher_IN1, LOW);
        digitalWrite(Pusher_IN2, HIGH);
        analogWrite(Pusher_ENA, 150);
    }
    Pusher_Go(0,0);
    delay(100);
    Pusher_Go(1,90);; //up
    delay(1300); //水平
    Pusher_Go(0,0);
    Serial.println(message);
}

//throw bowling ball (throw one ball at a time)
void releasing_bowling_three(int& time , int& status)
{
    int duration = millis() - start_time;
    if(time == 1)
    {
        if(status == 1)
        {
            while(digitalRead(pusherLimswit) == 1)
            {
                PusherDown(Pusher_status); //down
            }
            Pusher_Go(0,0);
            delay(200);
            Pusher_Go(1,150); //up
            delay(1300);
            Pusher_Go(0,0);
            status = 2;
            start_time = millis();
        }
        else if(status == 2 and duration < 3000 )
        {
            flywheel_task(2);
        }
        else if(status == 2 and duration > 3000 )
        {
            status = 3;
            start_time = millis();
        }
        else if(status == 3 and duration < 5000 )
        {
            int Motstatus = 2;
            downDC_task(Motstatus); //forward
            topStepper_task(Motstatus);
            flywheel_task(2);
        }
        else if(status == 3 and duration > 5000)
        {
            int DCstatus = 1;
            downDC_task(DCstatus); // stop
            flywheel_task(1);
            Serial.println(message);
            status = 0;
        }
    }
    else if(time == 2 )
    {
        if(status == 1 and duration < 3000)
        {
            flywheel_task(2); 
        }
        else if(status == 1 and duration > 3000)
        {
            status = 2;
            start_time = millis();
        }
        else if(status == 2 and duration < 5000 )
        {
            int DCstatus = 2;
            downDC_task(DCstatus); //forward
            flywheel_task(2);
        }
        else if(status == 2 and duration > 5000)
        {
            int DCstatus = 1;
            downDC_task(DCstatus); // stop
            flywheel_task(1);
            Serial.println(message);
            status = 0;
        }
    }
    else if(time == 3)
    {
        if(status == 1 and duration < 2000)
        {
            flywheel_task(2);
        }
        else if(status == 1 and duration > 2000)
        {
            status = 2;
            start_time = millis();
        }
        else if(status == 2 )
        {
            for(int i = 0 ; i < 2000 ; i++) 
            {
                flywheel_task(2);
                int STstatus = 2;
                topStepper_task(STstatus);//走22cm 
            }
            flywheel_task(1);
            int STstatus = 1;
            topStepper_task(STstatus);
            status = 0;
            time = 0;
            StandardPosi(); 
        } 
    }
}*/
