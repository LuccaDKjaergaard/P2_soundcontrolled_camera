#include <math.h>

const unsigned int SOUND_THRESHOLD = 700; //analog value for sound detection
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

const int PIN_INTERRUPT = 2;

//servo:
const int PIN_SERVO = 3;
volatile int servoPosition; //volatile because it is accessed by both loops

void setup() {
  Serial.begin(115200);
  while(!Serial); //wait for serial

  analogReadResolution(10);

  InitMicrophones();
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
      micMiddle.detected = HIGH;
      digitalWrite(PIN_INTERRUPT, HIGH);
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
  Reset();
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
  digitalWrite(PIN_INTERRUPT, LOW);
  micLeft.detected = LOW;
  micMiddle.detected = LOW;
  micRight.detected = LOW;
  soundState = unknown;
  for(int i = 0; i < ARRAY_LENGTH; i++) {
    soundArrayLeft[i] = 0;
    soundArrayRight[i] = 0;
  }

  Serial.println("All has been reset.");
}
