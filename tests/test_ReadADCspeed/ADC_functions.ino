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
  uint8_t highByte = SPI.transfer(0x00); //contains 2 unknown bits, the null bit, 5 highest-order bits
  uint8_t lowByte = SPI.transfer(0x00); //contains 7 lowest-order bits, 1 repeated bit

  digitalWrite(PIN_CS_ADC, HIGH);

  //highByte &= B00011111; //set unknown and null bits to 0
  //lowByte >>= 1; //shift the repeated bit out of existence
  //uint16_t adcValue = ((highByte << 7) | lowByte); //combine into 12 bit

  // Align to 12-bit:
  uint16_t adcValue = ((highByte << 8) | lowByte);
  adcValue = (adcValue >> 1) & 0x0FFF; //0x0FFF = 0000 1111 1111 1111

  return adcValue;
}
