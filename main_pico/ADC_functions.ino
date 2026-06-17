void InitADC() {
  //initialise chipselect:
  pinMode(ADC_CS, OUTPUT);
  digitalWrite(ADC_CS, HIGH); //deselect
  SPI1.setRX(ADC_MISO);
  //SPI1.setTX(ADC_MOSI);
  SPI1.setSCK(ADC_SCK);
  SPI1.begin();

  //initialise SPI with manually configured pins:
  //spi2.begin(PIN_CLK_ADC, PIN_MISO_ADC, PIN_MOSI_ADC, PIN_CS_ADC);
  SPI1.beginTransaction(SPISettings(1600000, MSBFIRST, SPI_MODE0));

  Serial.println("SPI1 (ADC) ready.");
}

uint16_t ReadADC() {
  digitalWrite(ADC_CS, LOW); //select

  //read 16 bits (2 x 8 clocks):
  uint8_t highByte = SPI1.transfer(0x00); //returns 3 unusable bits + 5 usable
  uint8_t lowByte = SPI1.transfer(0x00); //returns 7 usable bits + 1 unusable

  digitalWrite(ADC_CS, HIGH); //deselect

  //align to 12-bit:
  uint16_t adcValue = ((highByte << 8) | lowByte);
  adcValue = (adcValue >> 1) & 0x0FFF;

  return adcValue;
}