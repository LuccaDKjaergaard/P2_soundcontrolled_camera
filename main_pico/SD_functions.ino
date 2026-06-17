void InitSD() {
  //initialise SPI:
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH); //deselect
  SPI.setRX(SD_MISO);
  SPI.setTX(SD_MOSI);
  SPI.setSCK(SD_SCK);
  //SPI.begin();
  if(!SD.begin(SD_CS, SPI)) {Serial.println("Failed to init SD.");}

  //initialise SPI with manually configured pins:
  //spi3.begin(PIN_CLK, PIN_MISO, PIN_MOSI, PIN_CS_SD);

  Serial.println("SPI0 (SD card) ready.");
}

void WriteToSD() {
  File file = SD.open(PATH, FILE_WRITE);
    for(int i = backlogCnt; i < BACKLOGSIZE; i++) {
      file.print("B["); file.print(i); file.print("]: "); //B for backlog
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