//步進馬達 test
void setup() {

    pinMode(9,OUTPUT);  
    pinMode(12,OUTPUT); //DIR
    digitalWrite(12, HIGH);
}

void loop() {

        digitalWrite(9, HIGH);
        delayMicroseconds(2000);
        digitalWrite(9, LOW);
        delayMicroseconds(2000);
        
}
