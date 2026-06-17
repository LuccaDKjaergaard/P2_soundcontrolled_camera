#include <SD.h>
//#include <sd_defines.h>
//#include <sd_diskio.h>
#include <SPI.h>

//SPI definitions:
//SPI0 - SD card
#define SD_SCK 18
#define SD_MOSI 19
#define SD_MISO 16
#define SD_CS 17
//SPI1 - ADC
#define ADC_SCK 10
//#define ADC_MOSI 11
#define ADC_MISO 12
#define ADC_CS 13

#define PIN_ISR_TIMER 5
#define PIN_ISR_SOUND 6

#define BACKLOGSIZE 40000 //2 sec
#define FRONTLOGSIZE 60000 //3 sec
uint16_t backlog[BACKLOGSIZE]; //must be same size as what ReadADC() returns
uint16_t frontlog[FRONTLOGSIZE]; //must be same size as what ReadADC() returns
unsigned int backlogCnt = 0;
unsigned int frontlogCnt = 0;

//volatile because they are changed by ISR
volatile bool soundDetected = false;
volatile bool writeToSD = false;

#define PATH "/adc_out.txt" //could be changed to something more time-specific

void setup() {
  Serial.begin(115200);
  while(!Serial);

  InitSD();
  InitADC();

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
    detachInterrupt(digitalPinToInterrupt(PIN_ISR_TIMER));
    detachInterrupt(digitalPinToInterrupt(PIN_ISR_SOUND));
    digitalWrite(ADC_CS, HIGH); //deselect
    SPI1.endTransaction();
    //if(!SD.begin(SD_CS, SPI)) {Serial.println("Failed to init SD.");}
    digitalWrite(SD_CS, LOW); //select

    Serial.print("Writing to SD card...");
    WriteToSD();
    Serial.println("Successfully written to SD card");
    
    digitalWrite(SD_CS, HIGH); //deselct
    SPI1.beginTransaction(SPISettings(1600000, MSBFIRST, SPI_MODE0));
    digitalWrite(ADC_CS, LOW); //select
    Reset();
    attachInterrupt(digitalPinToInterrupt(PIN_ISR_SOUND), ISR_SOUND, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_ISR_TIMER), ISR_TIMER, RISING);
  }
}

void Reset() {
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