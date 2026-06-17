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

/*void WriteToSD() {
  File file = SD.open(PATH, O_WRITE | O_CREAT | O_TRUNC);

  // Write a simple header so you know the array sizes when reading back
  file.write((uint8_t)&backlogCnt, sizeof(backlogCnt));

  // Write arrays as raw binary blobs in one call each
  file.write((uint8_t)backlog, sizeof(backlog));
  file.write((uint8_t*)frontlog, sizeof(frontlog));

  file.close();
}*/

void WriteToSD() {
  // Allocate a buffer large enough for all your data
  //const size_t bufSize = sizeof(backlogCnt) + sizeof(backlog) + sizeof(frontlog);
  uint8_t buf1[sizeof(backlogCnt)], buf2[sizeof(backlog)], buf3[sizeof(frontlog)];
  //size_t offset = 0;

  // Copy data into buffer
  memcpy(buf1, &backlogCnt, sizeof(backlogCnt)); //offset += sizeof(backlogCnt);
  memcpy(buf2, backlog,     sizeof(backlog));    //offset += sizeof(backlog);
  memcpy(buf3, frontlog,    sizeof(frontlog));   //offset += sizeof(frontlog);

  // One single write call
  //File file = SD.open(PATH, O_WRITE | O_CREAT | O_TRUNC);
  File file = SD.open(PATH, FILE_WRITE | O_BINARY);
  //file.write(buf, bufSize);
  file.write(buf1, sizeof(buf1));
  file.write(buf2, sizeof(buf2));
  file.write(buf3, sizeof(buf3));
  file.close();
}

/*void WriteToSD() {
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
}*/