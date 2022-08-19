const byte c_IN1 = 3 ; //speed
const byte c_IN2 = 4; //direction
const byte d_IN1 = 5; //speed
const byte d_IN2 = 6; //direction
const byte button = 8;
void setup() {
  // put your setup code here, to run once:
   pinMode(c_IN1, OUTPUT);
    pinMode(c_IN2, OUTPUT);
    pinMode(d_IN1, OUTPUT);
    pinMode(d_IN2, OUTPUT);
    pinMode(button,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if  (digitalRead(button)== 1){
         analogWrite(c_IN1, 250);
            digitalWrite(c_IN2, LOW);
            analogWrite(d_IN1, 250);
            digitalWrite(d_IN2, HIGH);
    } else {
         analogWrite(c_IN1, 0);
            digitalWrite(c_IN2, LOW);
            analogWrite(d_IN1, 0);
            digitalWrite(d_IN2, HIGH);
    }
}
