//推桿 test
//推桿會自己停
int sta=0;
void setup() {
    Serial.begin(57600);
    pinMode(8,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(A4,INPUT_PULLUP);
}

void loop() {

//1 停
//0 換方向 重啟
    Serial.println(digitalRead(A4)); 
    if(digitalRead(A4) == 0)
    {   
        Serial.println(digitalRead(A4));
        digitalWrite(9,HIGH);
        digitalWrite(10,LOW);
        analogWrite(8,100);
        sta = 1;
        
    }else
    {
        digitalWrite(2, LOW);
        if(sta == 1)
        {
            sta =0;
        }
    }
}