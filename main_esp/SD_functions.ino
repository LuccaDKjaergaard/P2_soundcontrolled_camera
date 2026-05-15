void InitSD() {
  //initialise chipselect:
  pinMode(CS_SD, OUTPUT);
  digitalWrite(CS_SD, HIGH);

  //initialise SPI with manually configured pins:
  SPI.begin(CLKpin, MISOpin, MOSIpin, CS_SD);
  if(!SD.begin(CS_SD)) {Serial.println("Failed to init SD.");}
  
}

void WriteToSD() {
  File file = SD.open(PATH, FILE_WRITE);
    for (int i = 0; i < backlogCnt; i++) {
      file.print("B["); file.print(i); file.print("]: ");
      file.println(backlog[i]);
      //fix overflow
    }
    for (int i = 0; i < FRONTLOGSIZE; i++) {
      file.print("F["); file.print(i); file.print("]: ");
      file.println(frontlog[i]);
    }
    file.close();
}