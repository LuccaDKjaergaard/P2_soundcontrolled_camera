void InitSD() {
  //initialise SPI:
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH); //deselect

  SPI.setRX(SD_MISO);
  SPI.setTX(SD_MOSI);
  SPI.setSCK(SD_SCK);
  //SPI.begin();
  //if(!SD.begin(SD_CS, SPI)) {Serial.println("Failed to init SD.");}
  // SdFat requires explicit SPI config — 400 kHz for init is handled internally
  SdSpiConfig spiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(4), &SPI);

  //debug code for if an error occurs:
  if (!sd.begin(spiConfig)) {
    Serial.println("Failed to init SD.");
    //sd.initErrorHalt(&Serial); // prints the specific SD error code
    while(true) {}
  }

  Serial.println("SPI0 (SD card) ready.");
}

//uint8_t buf1[sizeof(backlogCnt)], buf2[sizeof(backlog)], buf3[sizeof(frontlog)];

/*void WriteToSD() {
  // Copy data into buffer
  memcpy(buf1, &backlogCnt, sizeof(backlogCnt));
  memcpy(buf2, backlog, sizeof(backlog));
  memcpy(buf3, frontlog, sizeof(frontlog));

  // One single write call
  file.open(PATH, O_WRONLY | O_CREAT | O_APPEND);
  file.write(buf1, sizeof(buf1));
  file.write(buf2, sizeof(buf2));
  file.write(buf3, sizeof(buf3));
  file.close();
  Serial.println("File closed.");
}*/

void WriteToSD() {
  //file.open(PATH, O_WRONLY | O_CREAT | O_APPEND);
  file.open(PATH, O_WRONLY | O_CREAT | O_TRUNC);

  file.print("Old backlog");
  for(int i = backlogCnt; i < BACKLOGSIZE; i++) {
    file.print(",");
    file.print(backlog[i]);
  }
  file.println();
  file.print("New backlog");
  for (int i = 0; i < backlogCnt; i++) {
    file.print(",");
    file.print(backlog[i]);
  }
  file.println();
  file.print("Frontlog");
  for (int i = 0; i < FRONTLOGSIZE; i++) {
    file.print(",");
    file.print(frontlog[i]);
  }
  file.close();
}