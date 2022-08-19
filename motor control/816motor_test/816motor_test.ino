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
#define button = 37;

void setup()
{
    Serial.begin(57600);
    pinMode(button,INPUT_PULLUP);
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

String message;
void action (int speed )
{
        if(message == "c2")
        {
            analogWrite(c_IN1, speed);
            digitalWrite(c_IN2, LOW);
            analogWrite(d_IN1, speed);
            digitalWrite(d_IN2, HIGH);
        }
        else if(message == "c3")
        {
            analogWrite(c_IN1, speed);
            digitalWrite(c_IN2, HIGH);
            analogWrite(d_IN1, speed);
            digitalWrite(d_IN2, LOW);
        }
        else if (message == "c1")
        {
            analogWrite(c_IN1, 0);
            digitalWrite(c_IN2, LOW);
            analogWrite(d_IN1, 0);
            digitalWrite(d_IN2, LOW);
        }
}

void loop()
{
    if (Serial.available() > 0)
    {
        message = Serial.readString();
        Serial.println(message);
        action(135);
    }
    
    if  (digitalRead(button)== 1){
         analogWrite(c_IN1, 135);
            digitalWrite(c_IN2, LOW);
            analogWrite(d_IN1, 135);
            digitalWrite(d_IN2, HIGH);
    } else {
         analogWrite(c_IN1, 0);
            digitalWrite(c_IN2, LOW);
            analogWrite(d_IN1, 0);
            digitalWrite(d_IN2, HIGH);
    }
}