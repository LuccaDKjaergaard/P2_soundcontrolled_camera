void InitADC() {
  //initialise chipselect:
  pinMode(PIN_CS_ADC, OUTPUT);
  digitalWrite(PIN_CS_ADC, HIGH);

  //initialise SPI with manually configured pins:
  spi2.begin(PIN_CLK_ADC, PIN_MISO_ADC, PIN_MOSI_ADC, PIN_CS_ADC);
  spi2.beginTransaction(SPISettings(1600000, MSBFIRST, SPI_MODE0));
}

uint16_t ReadADC() {
  digitalWrite(PIN_CS_ADC, LOW);

  //read 16 bits (2 x 8 clocks):
  uint8_t highByte = spi2.transfer(0x00); //returns 3 unusable bits + 5 usable
  uint8_t lowByte = spi2.transfer(0x00); //returns 7 usable bits + 1 unusable

  digitalWrite(PIN_CS_ADC, HIGH);

  //align to 12-bit:
  uint16_t adcValue = ((highByte << 8) | lowByte);
  adcValue = (adcValue >> 1) & 0x0FFF;

  return adcValue;
}