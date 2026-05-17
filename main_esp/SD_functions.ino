void InitSD() {
  //initialise chipselect:
  pinMode(PIN_CS_SD, OUTPUT);
  digitalWrite(PIN_CS_SD, HIGH);

  //initialise SPI with manually configured pins:
  spi3.begin(PIN_CLK, PIN_MISO, PIN_MOSI, PIN_CS_SD);
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