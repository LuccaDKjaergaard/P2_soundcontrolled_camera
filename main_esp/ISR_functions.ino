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
      //Serial.println("writeToSD = true");
    }
  } else if (!soundDetected) {
    if(backlogCnt >= BACKLOGSIZE) {
      backlogCnt = 0;
    }
    backlog[backlogCnt] = ADCBackLog();
    backlogCnt++;
  }
}