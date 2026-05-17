#include <SD.h>
#include <sd_defines.h>
#include <sd_diskio.h>

#include <SPI.h>


//GPIO Pins Waveshare ESP32-S3-Nano or ESP32-S3-zero
#define CS_ADC 21   //D10
#define CLKpin 48   //D13
#define MISOpin 47  //D12
#define MOSIpin 38  //D11

#define CS_SD 18  //D9

#define ISR_TIMER_PIN 5  //D2
#define ISR_SOUND_PIN 6  //D3

#define BACKLOGSIZE 20
#define FRONTLOGSIZE 40

//also create directory(?) for each day (can be done using __DATE__)
#define PATH "/adc_out.txt" //should be changed to something more time-specific

//DEBUG (working):
//unsigned char backlog[BACKLOGSIZE];
//unsigned char frontlog[FRONTLOGSIZE];
uint16_t backlog[BACKLOGSIZE];
uint16_t frontlog[FRONTLOGSIZE];
volatile bool soundDetected = false;
volatile bool writeToSD = false;

unsigned int frontlogCnt = 0;
unsigned int backlogCnt = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);  //wait for serial

  Serial.println("Wait for setup...");
  InitSD();
  InitADC();
  Serial.println("Setup done!");

  Serial.print("Size of backlog: ");
  Serial.print(sizeof(backlog));
  Serial.println(" bytes");
  Serial.print("Size of frontlog: ");
  Serial.print(sizeof(frontlog));
  Serial.println(" bytes");

  InitISR();
}

void loop() {
  if (writeToSD) {
    //noInterrupts();
    //order of functions is important here...
    detachInterrupt(digitalPinToInterrupt(ISR_TIMER_PIN));
    detachInterrupt(digitalPinToInterrupt(ISR_SOUND_PIN));
    digitalWrite(CS_ADC, HIGH);
    SPI.endTransaction();
    if(!SD.begin(CS_SD)) {Serial.println("Failed to init SD.");}
    digitalWrite(CS_SD, LOW);

    Serial.print("Writing to SD card...");
    WriteToSD();
    Serial.println("Successfully written to SD card");
    
    digitalWrite(CS_SD, HIGH);
    SPI.beginTransaction(SPISettings(1600000, MSBFIRST, SPI_MODE0));
    digitalWrite(CS_ADC, LOW);
    reset();
    //interrupts();
    attachInterrupt(digitalPinToInterrupt(ISR_SOUND_PIN), ISR_SOUND, RISING);
    attachInterrupt(digitalPinToInterrupt(ISR_TIMER_PIN), ISR_TIMER, RISING);
  }
}

void InitISR() {
  pinMode(ISR_SOUND_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ISR_SOUND_PIN), ISR_SOUND, RISING);
  pinMode(ISR_TIMER_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ISR_TIMER_PIN), ISR_TIMER, RISING);
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
