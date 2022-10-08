/* This will be stored in the arduino board ,and receive the msg sended from the python server */

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
const byte rDCmotor_IN1 = 32; //speed 
const byte rDCmotor_IN2 = 30; //direction
const byte lDCmotor_IN1 = 24; //speed 
const byte lDCmotor_IN2 = 26; //direction

// limit switch 都接NO (觸發時為0)
const byte topbLimswit = 4; //上後
const byte downbLimswit = 6; //下後
const byte downfLimswit = 5; //下前
const byte pusherLimswit = 7; //推桿

String message;
int topStepper_status;
int downDC_status;
int Pusher_status;
int flywheel_status;

int throwBasket_status = 0; 
int throwBowling_status = 0;

int takeBasket_times = 0;
int takeBowling_times = 0;
int throwBasket_times = 0;
int throwBowling_times = 0;

int start_time = 0;

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

        delay(400);
        while(digitalRead(topbLimswit) == 0)
        {
            // forward
            StepperGo(topStepper_CW,topStepper_CLK,0); 
        }
        status = 1;
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
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, 0);
        delay(400);
        while(digitalRead(downfLimswit) == 0)
        {
            //backward
            digitalWrite(downDC_IN1, LOW);
            digitalWrite(downDC_IN2, HIGH);
            analogWrite(downDC_ENA, 150);
        }
        status = 1;
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
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, 0);
        delay(400);
        while(digitalRead(downbLimswit) == 0)
        {
            //forward
            digitalWrite(downDC_IN1, HIGH);
            digitalWrite(downDC_IN2, LOW);
            analogWrite(downDC_ENA, 150);
        }
        status = 1;
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
        digitalWrite(rDCmotor_IN1, speedIn);
        digitalWrite(rDCmotor_IN2, LOW);
        digitalWrite(lDCmotor_IN1, speedIn);
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
        PusherStop();
        delay(400);
        while(digitalRead(pusherLimswit) == 0)
        {
            PusherUp();
        }
        status = 1; //stop
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

/* Programs about the task */

//standard position
void StandardPosi()
{
    Pusher_status = 2;
    topStepper_status = 3;
    downDC_status = 2;
    Serial.println(message);
}

//take bowlingball (take one ball at a time)
void takeBowling_three(int& time) 
{
    if(time == 1)
    {
        PusherUp(); //up
        delay(5750);
        PusherStop(); //stop
        while(digitalRead(downbLimswit) == 1)
        {
            //backward
            digitalWrite(downDC_IN1, LOW);
            digitalWrite(downDC_IN2, HIGH);
            analogWrite(downDC_ENA, 150);
        }
        while(digitalRead(downbLimswit) == 1)
        {
            //backward
            digitalWrite(downDC_IN1, LOW);
            digitalWrite(downDC_IN2, HIGH);
            analogWrite(downDC_ENA, 150);
        }
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, 0);
        delay(300);
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
        PusherStop();
        delay(2500);
        PusherUp(); //up
        delay(5500);
        PusherStop();
        Serial.println(message);
    } 
    else if(time == 2)
    {
        while(digitalRead(pusherLimswit) == 1)
        {
            PusherDown(Pusher_status); //down
        }
        PusherStop();
        delay(3000);
        PusherUp(); //up
        delay(3000);
        PusherStop();
        Serial.println(message);
      
    }
    else if(time ==3)
    {
        while(digitalRead(pusherLimswit) == 1)
        {
            PusherDown(Pusher_status); //down
        }
        PusherStop();
        delay(300);
        PusherUp(); //up
        delay(4500);
        PusherStop();
        Serial.println(message);
        time = 0;
    }
}

//take basketball (take one ball at a time)
void takeBasket_three(int& time) 
{
    if(time == 1)
    {
        PusherUp(); //up
        delay(5750);
        PusherStop(); //stop
        while(digitalRead(downbLimswit) == 1)
        {
            //backward
            digitalWrite(downDC_IN1, LOW);
            digitalWrite(downDC_IN2, HIGH);
            analogWrite(downDC_ENA, 150);
        }
        while(digitalRead(downbLimswit) == 1)
        {
            //backward
            digitalWrite(downDC_IN1, LOW);
            digitalWrite(downDC_IN2, HIGH);
            analogWrite(downDC_ENA, 150);
        }
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, 0);
        delay(300);
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
        PusherStop();
        delay(2500);
        PusherUp(); //up
        delay(5500);
        PusherStop();
        Serial.println(message);
    } 
    else if(time == 2)
    {
        while(digitalRead(pusherLimswit) == 1)
        {
            PusherDown(Pusher_status); //down
        }
        PusherStop();
        delay(3000);
        PusherUp(); //up
        delay(3000);
        PusherStop();
        Serial.println(message);
      
    }
    else if(time ==3)
    {
        while(digitalRead(pusherLimswit) == 1)
        {
            PusherDown(Pusher_status); //down
        }
        PusherStop();
        delay(300);
        PusherUp(); //up
        delay(4500);
        PusherStop();
        Serial.println(message);
        time = 0;
    }
}

//take basketball (take one ball at a time)
void takeBasket_Two_One(int& time) 
{
    if(time == 1)
    {
        PusherUp(); //up
        delay(5750);
        PusherStop(); //stop
        while(digitalRead(downbLimswit) == 1)
        {
            //backward
            digitalWrite(downDC_IN1, LOW);
            digitalWrite(downDC_IN2, HIGH);
            analogWrite(downDC_ENA, 150);
        }
        while(digitalRead(downbLimswit) == 1)
        {
            //backward
            digitalWrite(downDC_IN1, LOW);
            digitalWrite(downDC_IN2, HIGH);
            analogWrite(downDC_ENA, 150);
        }
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, LOW);
        analogWrite(downDC_ENA, 0);
        delay(300);
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
        PusherStop();
        delay(2500);
        PusherUp(); //up
        delay(5500);
        PusherStop();
        Serial.println(message);
    } 
    else if(time == 2)
    {
        while(digitalRead(pusherLimswit) == 1)
        {
            PusherDown(Pusher_status); //down
        }
        PusherStop();
        delay(3000);
        PusherUp(); //up
        delay(4000);
        PusherStop();
        Serial.println(message);
      
    }
    else if(time ==3)
    {
        PusherUp(); //up
        delay(5750);
        PusherStop(); //stop
        while(digitalRead(pusherLimswit) == 1)
        {
            PusherDown(Pusher_status); //down
        }
        PusherStop();
        delay(300);
        PusherUp(); //up
        delay(4500);
        PusherStop();
        Serial.println(message);
        time = 0;
    }
}


//throw basketball (throw one ball at a time)
void throwing_basketball_three(int& time , int& status)
{
    int duration = millis()-start_time ;
    if(time == 1)
    {
        if(status == 1)
        {
            PusherUp();
            delay(5000); 
            PusherStop();
            // delay(200);
            // PusherDown(Pusher_status);
            // delay(1440);
            // PusherStop();
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

//throw basketball (throw one ball at a time)
void throwing_basketball_Two_One(int& time , int& status)
{
    int duration = millis()-start_time ;
    if(time == 1)
    {
        if(status == 1)
        {
            PusherUp();
            delay(5000); 
            PusherStop();
            delay(200);
            PusherDown(Pusher_status);
            // delay(1440);
            delay(800);
            PusherStop();
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
            downDC_task(Motstatus); 
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
            PusherUp();
            delay(1000); 
            PusherStop();
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
        else if(status == 3 and duration < 3900 ) 
        {
            //downDC , topStepper , flywheel be activated at the same time 
            int DCstatus = 2;
            downDC_task(DCstatus); 
            int STstatus = 2;
            topStepper_task(STstatus);
            flywheel_task(2);
        }
        else if(status == 3 and duration > 3900)
        {
            int STstatus = 1;
            topStepper_task(STstatus);
            int DCstatus = 1;
            downDC_task(DCstatus); 
            flywheel_task(1);
            status = 0;
            Serial.println(message);
        }    
    }
    //中間有一次取球
    else if(time == 3)
    {
        if(status == 1)
        {
            PusherUp();
            delay(5000); 
            PusherStop();
            delay(200);
            PusherDown(Pusher_status);
            delay(1440);
            PusherStop();
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
        else if(status == 3 and duration < 12800 ) //從最底到最高
        {
            //downDC , topStepper , flywheel be activated at the same time 
            int DCstatus = 2;
            downDC_task(DCstatus); 
            int STstatus = 2;
            topStepper_task(STstatus);
            flywheel_task(2);
        }
        else if(status == 3 and duration > 12800)
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

//take bowling ball (take three balls in a row)
void taking_bowling()
{
    while (digitalRead(topbLimswit) == 1)
    {
        // backward
        StepperGo(topStepper_CW,topStepper_CLK,1);
    }
    delay(200);
    while(digitalRead(topbLimswit) == 0)
    {
        // forward
        StepperGo(topStepper_CW,topStepper_CLK,0); 
    }
    PusherUp(); //up
    delay(5000);
    PusherStop(); //stop
    while(digitalRead(downbLimswit) == 1)       //first ball start
    {
        //backward
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, HIGH);
        analogWrite(downDC_ENA, 150);
    }
    while(digitalRead(downbLimswit) == 1) //有時候退不到底 因此多確認一次
    {
        //backward
        digitalWrite(downDC_IN1, LOW);
        digitalWrite(downDC_IN2, HIGH);
        analogWrite(downDC_ENA, 150);
    }
    digitalWrite(downDC_IN1, LOW);  //downmotorstop
    digitalWrite(downDC_IN2, LOW);
    analogWrite(downDC_ENA, 0);
    delay(300);
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
    PusherStop();
    delay(1000);
    PusherUp(); //up
    delay(5000);
    PusherStop();
    for(int i=1 ; i<3 ; i++)
    {
        while(digitalRead(pusherLimswit) == 1)
        {
            PusherDown(Pusher_status); //down
        }
        PusherStop();
        delay(1000);
        PusherUp(); //up
        delay(5000);
        PusherStop();
    }
    Serial.println(message);
}

//throw bowling ball (throw one ball at a time)
void releasing_bowling_test(int& time , int& status)
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
            PusherStop();
            delay(200);
            PusherUp(); //up
            delay(1300);
            PusherStop();
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
        else if(status == 3 and duration < 4200 )
        {
            int DCstatus = 2;
            downDC_task(DCstatus); //forward
            flywheel_task(2);
        }
        else if(status == 3 and duration > 4200)
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
        else if(status == 2 and duration < 4200 )
        {
            int DCstatus = 2;
            downDC_task(DCstatus); //forward
            flywheel_task(2);
        }
        else if(status == 2 and duration > 4200)
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

            //this function will send the msg back to the python server , 
            //therefore no needed of the serial.println()
            // StandardPosi(); 
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
            takeBasket_Two_One(takeBasket_times);
            // takeBasket_three(takeBasket_times);
            break;
        case '2': 
            throwBasket_times++;
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
            start_time = millis();
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
    topStepper_task(topStepper_status);
    downDC_task(downDC_status);
    Pusher_task(Pusher_status);
    flywheel_task(flywheel_status); 

    /* more than two motor need to be activated at the same , therefore we put the function here */
    // throwing_basketball_three(throwBasket_times,throwBasket_status);
    throwing_basketball_Two_One(throwBasket_times,throwBasket_status);

    releasing_bowling_test(throwBowling_times, throwBowling_status);
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
