#include <SD.h>
#include <sd_defines.h>
#include <sd_diskio.h>

#include <SPI.h>


//GPIO Pins Waveshare ESP32-S3-Nano
#define CS_ADC 21   //D10
#define CLKpin 48   //D13
#define MISOpin 47  //D12
#define MOSIpin 38  //D11

#define CS_SD 18  //D9

#define ISR_TIMER_PIN 5  //D2
#define ISR_SOUND_PIN 6  //D3

#define BACKLOGSIZE 20000
#define FRONTLOGSIZE 40000

#define PATH "/adc_out.txt"

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
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial);  //wait for serial
  Serial.println("Wait for setup...");

  //initialise chipselects
  pinMode(CS_SD, OUTPUT);
  digitalWrite(CS_SD, HIGH);
  pinMode(CS_ADC, OUTPUT);
  digitalWrite(CS_ADC, HIGH);
  //Serial.println("CS initialised");

  //initialise SPI with manually configured pins
  SPI.begin(CLKpin, MISOpin, MOSIpin, CS_ADC);
  SPI.begin(CLKpin, MISOpin, MOSIpin, CS_SD);
  if(!SD.begin(CS_SD)) {Serial.println("Failed to init SD.");}
  //SPI.beginTransaction(SPISettings(1600000, MSBFIRST, SPI_MODE0));
  //Serial.println("SPI initialised");
  Serial.println("Setup done!");

  Serial.print("Size of backlog: ");
  Serial.print(sizeof(backlog));
  Serial.println(" bytes");
  Serial.print("Size of frontlog: ");
  Serial.print(sizeof(frontlog));
  Serial.println(" bytes");

  //initialise interrupts
  pinMode(ISR_SOUND_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ISR_SOUND_PIN), ISR_SOUND, RISING);
  pinMode(ISR_TIMER_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ISR_TIMER_PIN), ISR_TIMER, RISING);
  //Serial.println("Interrupts initialised");
}

void loop() {
  //Serial.print("writeToSD = "); Serial.println(writeToSD);
  if (writeToSD) {
    //noInterrupts();
    detachInterrupt(digitalPinToInterrupt(ISR_TIMER_PIN));
    detachInterrupt(digitalPinToInterrupt(ISR_SOUND_PIN));
    Serial.println("hej hej :)");
    digitalWrite(CS_ADC, HIGH);
    digitalWrite(CS_SD, LOW);
    File file = SD.open(PATH, FILE_WRITE);
    for (int i = 0; i < backlogCnt; i++) {
      file.print("B["); file.print(i); file.print("]: ");
      file.println(backlog[i]);
      //fix overflow
    }
    for (int i = 0; i < FRONTLOGSIZE; i++) {
      file.print("F["); file.print(i); file.print("]: ");
      file.println(frontlog[i]);
    }
    file.close();
    digitalWrite(CS_SD, HIGH);
    digitalWrite(CS_ADC, LOW);
    Serial.println("Successfully written to SD card");
    reset();
    //interrupts();
    attachInterrupt(digitalPinToInterrupt(ISR_SOUND_PIN), ISR_SOUND, RISING);
    attachInterrupt(digitalPinToInterrupt(ISR_TIMER_PIN), ISR_TIMER, RISING);
  }
}

void ISR_SOUND() {
  soundDetected = true;
}

void ISR_TIMER() {

  if (soundDetected) {
    if (frontlogCnt < FRONTLOGSIZE) {
      frontlog[frontlogCnt] = ADCFrontLog();
      frontlogCnt++;
    } else if (frontlogCnt >= FRONTLOGSIZE) {
      writeToSD = true;
      //Serial.println("writeToSD = true");
    }
  } else if (!soundDetected) {
    if (backlogCnt < BACKLOGSIZE) {
      backlog[backlogCnt] = ADCBackLog();
      backlogCnt++;
    }
    //Todo: fix overflow
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

uint16_t ADCFrontLog() {

  digitalWrite(CS_ADC, LOW);

  // Read 16 bits (2 x 8 clocks)
  uint8_t highByte = SPI.transfer(0x00);
  uint8_t lowByte = SPI.transfer(0x00);

  digitalWrite(CS_ADC, HIGH);

  uint16_t adcValue = ((highByte << 8) | lowByte);

  // Align to 12-bit
  adcValue = (adcValue >> 1) & 0x0FFF;
  Serial.print("To frontLog: ");
  Serial.println(adcValue);
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

  Serial.print("To backLog: ");
  Serial.println(adcValue);
  return adcValue;
}
