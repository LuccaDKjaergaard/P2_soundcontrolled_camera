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
    if (backlogCnt < BACKLOGSIZE) {
      backlog[backlogCnt] = ADCBackLog();
      backlogCnt++;
    }
    //Todo: fix overflow
  }
}