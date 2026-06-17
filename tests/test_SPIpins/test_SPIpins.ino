#include <SD.h>
//#include <sd_defines.h>
//#include <sd_diskio.h>
#include <SPI.h>

// SPI0 — SD card
#define SD_SCK  18
#define SD_MOSI 19
#define SD_MISO 16
#define SD_CS   17

// SPI1 — external SPI ADC (MCP3204)
#define ADC_SCK  10
#define ADC_MOSI 11
#define ADC_MISO 12
#define ADC_CS   13

//SPIClass spi2(SPI); //ADC SPI class
//SPIClass spi3(SPI1); //SD SPI class

void setup() {
  Serial.begin(115200);
  while(!Serial);

  Serial.println(MOSI); //RPI PICO-2W = GP19
  Serial.println(MISO); //RPI PICO-2W = GP16
  Serial.println(SS); //RPI PICO-2W = GP17
  Serial.println(SCK); //RPI PICO-2W = GP18

  SPI.setRX(SD_MISO);
  SPI.setTX(SD_MOSI);
  SPI.setSCK(SD_SCK);
  SPI.begin();

  SPI1.setRX(ADC_MISO);
  SPI1.setTX(ADC_MOSI);
  SPI1.setSCK(ADC_SCK);
  SPI1.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}