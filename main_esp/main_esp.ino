#include <SD.h>
#include <sd_defines.h>
#include <sd_diskio.h>

#include <SPI.h>

//Split up the 2 possible SPI connections
SPIClass spi2(FSPI); //ADC SPI class
SPIClass spi3(HSPI); //SD SPI class

//GPIO Pins Waveshare ESP32-S3-Nano or ESP32-S3-zero
//SD card
#define PIN_CS_SD 21   //D10
#define PIN_CLK 48   //D13
#define PIN_MISO 47  //D12
#define PIN_MOSI 38  //D11
//MCP3201-C
#define PIN_CS_ADC 18  //D9
#define PIN_MISO_ADC 17 //D8
#define PIN_CLK_ADC 10 //D7
#define PIN_MOSI_ADC 9 //

#define PIN_ISR_TIMER 5  //D2
#define PIN_ISR_SOUND 6  //D3

#define BACKLOGSIZE 40000 //2 sec
#define FRONTLOGSIZE 60000 //3 sec
uint16_t backlog[BACKLOGSIZE]; //must be same size as what ReadADC() returns
uint16_t frontlog[FRONTLOGSIZE]; //must be same size as what ReadADC() returns
unsigned int backlogCnt = 0;
unsigned int frontlogCnt = 0;

//volatile because they are changed by ISR
volatile bool soundDetected = false;
volatile bool writeToSD = false;

#define PATH "/adc_out.txt" //should be changed to something more time-specific

void setup() {
  Serial.begin(115200);
  while (!Serial); //wait for serial

  //Serial.println("Wait for setup...");
  InitSD();
  InitADC();
  //Serial.println("Setup done!");

  Serial.print("Size of backlog: ");
  Serial.print(sizeof(backlog));
  Serial.println(" bytes.");
  Serial.print("Size of frontlog: ");
  Serial.print(sizeof(frontlog));
  Serial.println(" bytes.");

  InitISR();
}

void loop() {
  if (writeToSD) {
    //order of functions is important here...
    detachInterrupt(digitalPinToInterrupt(PIN_ISR_TIMER));
    detachInterrupt(digitalPinToInterrupt(PIN_ISR_SOUND));
    digitalWrite(PIN_CS_ADC, HIGH);
    spi2.endTransaction();
    if(!SD.begin(PIN_CS_SD, spi3)) {Serial.println("Failed to init SD.");}
    digitalWrite(PIN_CS_SD, LOW);

    Serial.print("Writing to SD card...");
    WriteToSD();
    Serial.println("Successfully written to SD card");
    
    digitalWrite(PIN_CS_SD, HIGH);
    spi2.beginTransaction(SPISettings(1600000, MSBFIRST, SPI_MODE0));
    digitalWrite(PIN_CS_ADC, LOW);
    reset();
    attachInterrupt(digitalPinToInterrupt(PIN_ISR_SOUND), ISR_SOUND, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_ISR_TIMER), ISR_TIMER, RISING);
  }
}

void reset() {
  frontlogCnt = 0;
  backlogCnt = 0;
  for (int i = 0; i < BACKLOGSIZE; i++) {
    backlog[i] = 0;
  }
  for (int i = 0; i < FRONTLOGSIZE; i++) {
    frontlog[i] = 0;
  }
  soundDetected = false;
  writeToSD = false;
}
