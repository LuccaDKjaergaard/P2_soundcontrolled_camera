void InitISR() {
  pinMode(ISR_SOUND_PIN, INPUT);
  pinMode(ISR_TIMER_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ISR_SOUND_PIN), ISR_SOUND, RISING);
  attachInterrupt(digitalPinToInterrupt(ISR_TIMER_PIN), ISR_TIMER, RISING);
}

void ISR_SOUND() {
  soundDetected = true;
}

void ISR_TIMER() {
  if (soundDetected) {
    if (frontlogCnt < FRONTLOGSIZE) {
      frontlog[frontlogCnt] = ADCFrontLog();
      frontlogCnt++;
    } else if (frontlogCnt >= FRONTLOGSIZE) {
      writeToSD = true;
      Serial.println("writeToSD = true");
    }
  } else if (!soundDetected) {
    if(backlogCnt >= BACKLOGSIZE) {
      backlogCnt = 0;
    }
    backlog[backlogCnt] = ADCBackLog();
    backlogCnt++;
  }
}