#include<Servo.h>
Servo myservo;
void setup() {
 
  Serial.begin(57600);
  myservo.attach(A1); 
  myservo.write(0);
}
void loop() {
  int i;
  char chr;
  if (Serial.available()) {
  while ((chr = Serial.read()) != '\n') {
    if (chr == '1' ) { 
        myservo.write(0);
      }
    if(chr=='2'){
      for(i=1;i<=180;i++)
      {
          myservo.write(i);
          Serial.print(i);
          delay(125);
       }
      }
    }
  }
}