void InitISR() {
  pinMode(PIN_ISR_SOUND, INPUT);
  pinMode(PIN_ISR_TIMER, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_ISR_SOUND), ISR_SOUND, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ISR_TIMER), ISR_TIMER, RISING);
}

void ISR_SOUND() {
  soundDetected = true;
}

void ISR_TIMER() {
  if (soundDetected) {
    if (frontlogCnt < FRONTLOGSIZE) {
      frontlog[frontlogCnt] = ReadADC();
      frontlogCnt++;
    } else if (frontlogCnt >= FRONTLOGSIZE) {
      writeToSD = true;
      Serial.println("writeToSD = true");
    }
  } else if (!soundDetected) {
    if(backlogCnt >= BACKLOGSIZE) {
      backlogCnt = 0;
    }
    backlog[backlogCnt] = ReadADC();
    backlogCnt++;
  }
}