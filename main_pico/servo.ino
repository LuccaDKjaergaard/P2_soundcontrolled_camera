void InitServo() {
  pinMode(PIN_SERVO, OUTPUT);
  UpdateServoPosition(90); //place servo in middle
}

//convert angle to servo signal length:
void UpdateServoPosition(int angle) {
  int valueMin = 400;
  int valueMax = 2400;
  servoPosition = map(angle, 0, 180, valueMin, valueMax); //update global variable
}