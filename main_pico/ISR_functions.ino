void InitISR() {
  pinMode(PIN_ISR_TIMER, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_ISR_TIMER), ISR_TIMER, RISING);
}

void ISR_TIMER() {
  if (soundDetected) {
    if (frontlogCnt < FRONTLOGSIZE) {
      frontlog[frontlogCnt] = analogRead(micMiddle.pin);
      frontlogCnt++;
    } else if (frontlogCnt >= FRONTLOGSIZE) {
      writeToSD = true;
      Serial.println("writeToSD = true");
    }
  } else if (!soundDetected) {
    if(backlogCnt >= BACKLOGSIZE) {
      backlogCnt = 0;
    }
    backlog[backlogCnt] = analogRead(micMiddle.pin);
    backlogCnt++;
  }
}