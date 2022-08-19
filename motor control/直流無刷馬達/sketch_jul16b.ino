#define a A1
#define b A3

void setup() {
Serial.begin(9600);

pinMode(a,OUTPUT);
pinMode(b,OUTPUT);
}

void stop()
{
  analogWrite(a,0);
  digitalWrite(b,HIGH);
}

void move()
{
    analogWrite(a,150);
    digitalWrite(b,HIGH);
    //HIGH LOW順時針
}

void loop() {
  stop();
}  

  
