#include <Servo.h>

const int pinServo = 9;
Servo servo;

void setup() {
  pinMode(pinServo, OUTPUT);
  servo.attach(pinServo);
}

void loop() {
  servo.write(0);
  delay(1000);
  servo.write(90);
  delay(1000);
  servo.write(180);
  delay(1000);
}
