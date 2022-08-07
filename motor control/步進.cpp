//步進馬達配緊急開關
//TB6560
int sta=0;
bool i = 1;
void setup() {
    Serial.begin(57600);
    pinMode(2,OUTPUT); //step clk+
    pinMode(7,OUTPUT); //DIR cw+
    digitalWrite(2, LOW);
    digitalWrite(7, LOW);
    pinMode(A4,INPUT_PULLUP);
}

void loop() {

//1 停
//0 換方向 重啟
    Serial.println(digitalRead(A4)); 
    if(digitalRead(A4) == 0)
    {   
        Serial.println(digitalRead(A4));
        digitalWrite(7, LOW);
        digitalWrite(2, HIGH);
        delayMicroseconds(500);
        digitalWrite(2, LOW);
        delayMicroseconds(500);
        
        sta = 1;
        
    }else
    {
        digitalWrite(2, LOW);
        if(sta == 1)
        {
            i = !i;
            sta =0;
        }
    }
}