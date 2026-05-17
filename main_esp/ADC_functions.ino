void InitADC() {
  //initialise chipselect:
  pinMode(CS_ADC, OUTPUT);
  digitalWrite(CS_ADC, HIGH);

  //initialise SPI with manually configured pins:
  SPI.begin(CLKpin, MISOpin, MOSIpin, CS_ADC);
  SPI.beginTransaction(SPISettings(1600000, MSBFIRST, SPI_MODE0));
}

uint16_t ADCFrontLog() {
  digitalWrite(CS_ADC, LOW);

  // Read 16 bits (2 x 8 clocks)
  uint8_t highByte = SPI.transfer(0x00);
  uint8_t lowByte = SPI.transfer(0x00);

  digitalWrite(CS_ADC, HIGH);

  uint16_t adcValue = ((highByte << 8) | lowByte);

  // Align to 12-bit
  adcValue = (adcValue >> 1) & 0x0FFF;

  return adcValue;
}

uint16_t ADCBackLog() {
  digitalWrite(CS_ADC, LOW);

  // Read 16 bits (2 x 8 clocks)
  uint8_t highByte = SPI.transfer(0x00);
  uint8_t lowByte = SPI.transfer(0x00);

  digitalWrite(CS_ADC, HIGH);

  uint16_t adcValue = ((highByte << 8) | lowByte);

  // Align to 12-bit
  adcValue = (adcValue >> 1) & 0x0FFF;

  return adcValue;
}