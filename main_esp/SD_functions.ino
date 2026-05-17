void InitSD() {
  //initialise chipselect:
  pinMode(CS_SD, OUTPUT);
  digitalWrite(CS_SD, HIGH);

  //initialise SPI with manually configured pins:
  SPI.begin(CLKpin, MISOpin, MOSIpin, CS_SD);
  //Serial.print("Waiting for SD init...");
  //if(!SD.begin(CS_SD)) {Serial.println("Failed to init SD.");}
  //Serial.println("SD init done!");
}

void WriteToSD() {
  File file = SD.open(PATH, FILE_WRITE);
    for(int i = backlogCnt; i < BACKLOGSIZE; i++) {
      file.print("B["); file.print(i); file.print("]: ");
      file.println(backlog[i]);
    }
    for (int i = 0; i < backlogCnt; i++) {
      file.print("B["); file.print(i); file.print("]: "); //B for backlog
      file.println(backlog[i]);
    }
    for (int i = 0; i < FRONTLOGSIZE; i++) {
      file.print("F["); file.print(i); file.print("]: "); //F for frontlog
      file.println(frontlog[i]);
    }
    file.close();
}