#include <math.h>
#include <SD.h>
#include <SdFat.h>
#include <SPI.h>

//Microphone array stuffs:
const unsigned int SOUND_THRESHOLD = 750; //analog value for sound detection
const int ARRAY_LENGTH = 2500;
int soundArrayLeft[ARRAY_LENGTH];
int soundArrayRight[ARRAY_LENGTH];

struct MicrophoneTp {
  int pin;
  unsigned int baseline;
  bool detected = LOW;
  unsigned long detectedTime;
};
struct MicrophoneTp micLeft, micMiddle, micRight;

enum SoundStateTp {
  fromLeft,
  fromRight,
  unknown
};
enum SoundStateTp soundState = unknown;

//servo:
const int PIN_SERVO = 3;
volatile int servoPosition; //volatile because it is accessed by both loops

//SPI0 definitions for SD-card:
#define SD_SCK 18
#define SD_MOSI 19
#define SD_MISO 16
#define SD_CS 17

//ISR's:
#define PIN_ISR_TIMER 5

//Sound logging:
#define BACKLOGSIZE 40000 //2 seconds
#define FRONTLOGSIZE 60000 //3 seconds
uint16_t backlog[BACKLOGSIZE]; //must be at least 10 bits
uint16_t frontlog[FRONTLOGSIZE]; //must be at least 10 bits
unsigned int backlogCnt = 0; //must be at least BACKLOGSIZE
unsigned int frontlogCnt = 0; //must be at least FRONTLOGSIZE

//volatile because they are changed by ISR
volatile bool soundDetected = false;
volatile bool writeToSD = false;

SdFat sd; //init SD-card
SdFile file; //init file
#define PATH "/adc_out.csv" //could be changed to something more time-specific

void setup() {
  Serial.begin(115200);
  while(!Serial); //wait for serial

  analogReadResolution(10);

  InitMicrophones();
  InitSD();

  Serial.print("Size of backlog: ");
  Serial.print(sizeof(backlog));
  Serial.println(" bytes.");
  Serial.print("Size of frontlog: ");
  Serial.print(sizeof(frontlog));
  Serial.println(" bytes.");

  InitISR();
}

void setup1() {
  InitServo();
}

void loop() {
  topOfLoop:

  //wait for sound on middle mic
  unsigned long counter = 0;
  while(!micMiddle.detected) {
    if(counter >= ARRAY_LENGTH) {counter = 0;}
    soundArrayLeft[counter] = analogRead(micLeft.pin);
    soundArrayRight[counter] = analogRead(micRight.pin);
    counter++;

    if(analogRead(micMiddle.pin) > SOUND_THRESHOLD || analogRead(micMiddle.pin) < (1023 - SOUND_THRESHOLD)) {
      micMiddle.detectedTime = micros();
      soundDetected = true;
      micMiddle.detected = HIGH;
      Serial.println("Sound detected!");
    }
  }

  //wait for sound on outer mics
  unsigned long timer = millis();
  const unsigned int TIMER = 100;
  int analogLeft = 0;
  int analogRight = 0;
  unsigned long micro;
  //wait for sound detection on both mics or time runs out
  while((!micLeft.detected || !micRight.detected) && (millis() - timer < TIMER)) {
    micro = micros();
    analogLeft = analogRead(micLeft.pin);
    analogLeft = analogRead(micLeft.pin);
    analogRight = analogRead(micRight.pin);
    analogRight = analogRead(micRight.pin);

    if(analogLeft > SOUND_THRESHOLD || analogLeft < (1023 - SOUND_THRESHOLD)) {
      micLeft.detectedTime = micro;
      micLeft.detected = HIGH;
    }
    if(analogRight > SOUND_THRESHOLD || analogRight < (1023 - SOUND_THRESHOLD)) {
      micRight.detectedTime = micro;
      micRight.detected = HIGH;
    }
  }
  //if timer ran out:
  if(millis() - timer > TIMER) {
    Serial.println("Time ran out.");
    Reset();
    goto topOfLoop;
  }

  //determine sound direction
  for(int i = counter; i < ARRAY_LENGTH; i++) {
    if(soundArrayLeft[i] > SOUND_THRESHOLD || soundArrayLeft[i] < (1023 - SOUND_THRESHOLD)) {
      soundState = fromLeft;
      Serial.println("Sound is coming from the left!");
      goto breakOut;
    }
    if(soundArrayRight[i] > SOUND_THRESHOLD || soundArrayRight[i] < (1023 - SOUND_THRESHOLD)) {
      soundState = fromRight;
      Serial.println("Sound is coming from the right!");
      goto breakOut;
    }
  }
  for(int i = 0; i < counter; i++) {
    if(soundArrayLeft[i] > SOUND_THRESHOLD || soundArrayLeft[i] < (1023 - SOUND_THRESHOLD)) {
      soundState = fromLeft;
      Serial.println("Sound is coming from the left!");
      goto breakOut;
    }
    if(soundArrayRight[i] > SOUND_THRESHOLD || soundArrayRight[i] < (1023 - SOUND_THRESHOLD)) {
      soundState = fromRight;
      Serial.println("Sound is coming from the right!");
      goto breakOut;
    }
  }
  soundState = unknown; //if all else fails
  breakOut:

  int soundAngle = CalculateSoundAngle();
  Serial.print("soundAngle: "); Serial.println(soundAngle);
  UpdateServoPosition(soundAngle);
  
  Serial.println("Waiting for frontlog...");
  while(true) {
    if(writeToSD) {
      detachInterrupt(digitalPinToInterrupt(PIN_ISR_TIMER));

      digitalWrite(SD_CS, LOW); //select
      Serial.print("Writing to SD card...");
      WriteToSD();
      Serial.println("Successfully written to SD card");
      digitalWrite(SD_CS, HIGH); //deselect

      Reset();
      attachInterrupt(digitalPinToInterrupt(PIN_ISR_TIMER), ISR_TIMER, RISING);
      break;
    }
  }
  delay(2000);
}

void loop1() {
  digitalWrite(PIN_SERVO, HIGH);
  delayMicroseconds(servoPosition);
  digitalWrite(PIN_SERVO, LOW);
  delay(17);
  delayMicroseconds(3000 - servoPosition);
}

void Reset() {
  //reset mic array stuffs:
  micLeft.detected = LOW;
  micMiddle.detected = LOW;
  micRight.detected = LOW;
  soundState = unknown;
  for(int i = 0; i < ARRAY_LENGTH; i++) {
    soundArrayLeft[i] = 0;
    soundArrayRight[i] = 0;
  }

  //reset SD stuffs:
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

  Serial.println("All has been reset.");
}
