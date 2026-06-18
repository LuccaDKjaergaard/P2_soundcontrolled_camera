void InitSD() {
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH); //deselect

  SPI.setRX(SD_MISO);
  SPI.setTX(SD_MOSI);
  SPI.setSCK(SD_SCK);
  
  SdSpiConfig spiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(4), &SPI);
  if(!sd.begin(spiConfig)) {Serial.println("Failed to init SD.");}

  Serial.println("SPI0 (SD card) ready.");
}

//uint8_t buf1[sizeof(backlogCnt)], buf2[sizeof(backlog)], buf3[sizeof(frontlog)];

/*void WriteToSD() {
  //copy data into buffers:
  memcpy(buf1, &backlogCnt, sizeof(backlogCnt));
  memcpy(buf2, backlog, sizeof(backlog));
  memcpy(buf3, frontlog, sizeof(frontlog));

  //only a few .write calls:
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
