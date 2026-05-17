void InitADC() {
  //initialise chipselect:
  pinMode(PIN_CS_ADC, OUTPUT);
  digitalWrite(PIN_CS_ADC, HIGH);

  //initialise SPI with manually configured pins:
  SPI.begin(PIN_CLK, PIN_MISO, PIN_MOSI, PIN_CS_ADC);
  SPI.beginTransaction(SPISettings(1600000, MSBFIRST, SPI_MODE0));
}

uint16_t ReadADC() {
  digitalWrite(PIN_CS_ADC, LOW);

  // Read 16 bits (2 x 8 clocks)
  uint8_t highByte = SPI.transfer(0x00);
  uint8_t lowByte = SPI.transfer(0x00);

  digitalWrite(PIN_CS_ADC, HIGH);

  uint16_t adcValue = ((highByte << 8) | lowByte);

  // Align to 12-bit
  adcValue = (adcValue >> 1) & 0x0FFF;

  return adcValue;
}
