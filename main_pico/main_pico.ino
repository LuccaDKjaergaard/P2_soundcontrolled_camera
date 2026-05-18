#include <math.h>

const unsigned int SOUND_THRESHOLD = 900; //analog value for sound detection - may not get below 1023/2 = 550 (rundet op)
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
int servoPosition;

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
  while(!micMiddle.detected) {
    if(analogRead(micMiddle.pin) > SOUND_THRESHOLD) {
      micMiddle.detectedTime = micros();
      micMiddle.detected = HIGH;
      Serial.println("Sound detected!");
    }
  }

  //wait for sound on outer mics
  unsigned long timer = millis();
  const unsigned int TIME = 500;
  int analogLeft = 0;
  int analogRight = 0;
  while(!micLeft.detected && !micRight.detected && (millis() - timer < TIME)) {
    //analogLeft = analogRead(micLeft.pin);
    analogLeft = analogRead(micLeft.pin);
    //analogRight = analogRead(micRight.pin);
    analogRight = analogRead(micRight.pin);
    unsigned long micro = micros();

    if(analogLeft > SOUND_THRESHOLD) {
      micLeft.detectedTime = micro;
      micLeft.detected = HIGH;
      soundState = fromRight;
      Serial.println("Sound is coming from the right!");
      break;
    }

    if(analogRight > SOUND_THRESHOLD) {
      micRight.detectedTime = micro;
      micRight.detected = HIGH;
      soundState = fromLeft;
      Serial.println("Sound is coming from the left!");
      break;
    }
  }

  //if both outer mics were detected (aka SOUND_THRESHOLD is too low)
  if(micLeft.detected && micRight.detected) {
    if(analogLeft > analogRight) {
      soundState = fromLeft;
    } else if(analogLeft < analogRight) {
      soundState = fromRight;
    }
  }
  
  if(millis() - timer > TIME) { //if time ran out
    Serial.println("Time ran out.");
    Reset();
    goto topOfLoop;
  }

  int soundAngle = CalculateSoundAngle();
  Serial.print("soundAngle: "); Serial.println(soundAngle);
  //UpdateServoPosition(soundAngle);
  Reset();
}

/*void loop1() {
  digitalWrite(PIN_SERVO, HIGH);
  delayMicroseconds(servoPosition);
  digitalWrite(PIN_SERVO, LOW);
  delay(17);
  delayMicroseconds(3000 - servoPosition);
}*/

void Reset() {
  micLeft.detected = LOW;
  micMiddle.detected = LOW;
  micRight.detected = LOW;
  soundState = unknown;
  Serial.println("All has been reset.");
  //goto topOfLoop;
}
