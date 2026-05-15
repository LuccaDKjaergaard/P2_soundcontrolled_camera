#include <math.h>
#include <Servo.h>

#define PIN_MIC1 A0
#define PIN_MIC2 A2
#define PIN_MIC3 A3

const unsigned int SPEED_OF_SOUND = 343; //m/s = mm/ms = microm/micros
const unsigned int MIC_DISTANCE = 120000; //micro m
const unsigned int SOUND_THRESHOLD = 700; //analog value for sound detection
unsigned long soundDetectionTimes[3];
enum SoundStateTp {
  fromLeft,
  fromMiddle,
  fromRight,
  unknown
};

Servo servo; //initialise servo motor

void setup() {
  Serial.begin(115200);
  while(!Serial);

  pinMode(PIN_MIC1, INPUT);
  pinMode(PIN_MIC2, INPUT);
  pinMode(PIN_MIC3, INPUT);
  servo.attach(9);
  servo.write(90); //place servo in middle

  //create interrupts (MEGA has 2, 3, 18, 19)
}

void loop() {
  //record time of sound detection:
  //can this be done smarter with krnl? Issue if sound is coming from the right, because the loop is repeated and PIN_MIC3 then gets a new value because of echos
  if(analogRead(PIN_MIC3) > SOUND_THRESHOLD) {soundDetectionTimes[2] = micros();}
  
  if(analogRead(PIN_MIC2) > SOUND_THRESHOLD) {soundDetectionTimes[1] = micros();}
  if(analogRead(PIN_MIC1) > SOUND_THRESHOLD) {soundDetectionTimes[0] = micros();}
  

  //check whether sound has been detected in the last second:
  const int TIME_FRAME = 1000;
  if(micros() - soundDetectionTimes[0] < TIME_FRAME
  && micros() - soundDetectionTimes[1] < TIME_FRAME
  && micros() - soundDetectionTimes[2] < TIME_FRAME) {
    Serial.println("New sound detected!");
    SoundStateTp soundState = DetermineSoundState();
    Serial.println(soundState);
    
    double servoAngle = UpdateAngles(soundState);
    Serial.println(servoAngle);
    //servo.write(map(servoAngle, 0, 180, 180, 0));
    servo.write(servoAngle);
    delay(1000); //wait for signal to settle after servo movement
  }
}

SoundStateTp DetermineSoundState() {
  if(soundDetectionTimes[0] < soundDetectionTimes[1] && soundDetectionTimes[1] < soundDetectionTimes[2]) {
    Serial.println("Sound is coming from the left of the board.");
    //soundState = fromLeft;
    return fromLeft;
  } else if(soundDetectionTimes[0] > soundDetectionTimes[1] && soundDetectionTimes[1] < soundDetectionTimes[2]) {
    Serial.println("Sound is coming from in front of the board.");
    //soundState = fromMiddle;
    return fromMiddle;
  } else if(soundDetectionTimes[0] > soundDetectionTimes[1] && soundDetectionTimes[1] > soundDetectionTimes[2]) {
    Serial.println("Sound is coming from the right of the board.");
    //soundState = fromRight;
    return fromRight;
  }
  //else:
  Serial.println("IDK where the sound is coming from.");
  //soundState = unknown;
  return unknown;
}

double UpdateAngles(SoundStateTp soundState) {
  unsigned long tdoa, tdoa1, tdoa2;
  unsigned long tdoaDistance, tdoaDistance1, tdoaDistance2; //long long to avoid overflow when SPEED_OF_SOUND*tdoa
  double angle_for_cos, angle_rad, angle_deg;

  switch(soundState) {
    case fromLeft:
      //calculate middle acute angle
      tdoa = soundDetectionTimes[1] - soundDetectionTimes[0];
      Serial.print("tdoa: "); Serial.println(tdoa);
      tdoaDistance = SPEED_OF_SOUND * tdoa; //returns μm/μs * μs = μm
      Serial.print("tdoaDistance: "); Serial.println(tdoaDistance);
      angle_for_cos = double(tdoaDistance) / double(MIC_DISTANCE);
      Serial.print("angle_for_cos: "); Serial.println(angle_for_cos);
      angle_rad = acos(angle_for_cos); //returns angle in radians
      Serial.print("angle_rad: "); Serial.println(angle_rad);
      angle_deg = angle_rad * 180.0 / M_PI; //returns angle in degrees
      Serial.print("angle_deg: "); Serial.println(angle_deg);
      return angle_deg;
      break;
    case fromMiddle:
      //calculate both outer angles
      tdoa1 = soundDetectionTimes[0] - soundDetectionTimes[1];
      tdoa2 = soundDetectionTimes[2] - soundDetectionTimes[1];
      tdoaDistance1 = SPEED_OF_SOUND * tdoa1;
      tdoaDistance2 = SPEED_OF_SOUND * tdoa2;
      double angleOuter1_rad = acos(double(tdoaDistance1) / double(MIC_DISTANCE)); //radians
      double angleOuter1_deg = angleOuter1_rad * 180.0 / M_PI; //degrees
      double angleOuter2_rad = acos(double(tdoaDistance2) / double(MIC_DISTANCE)); //radians
      double angleOuter2_deg = angleOuter2_rad * 180.0 / M_PI; //degrees

      //calculate both middle angles
      double angleInner1 = (180.0 - 90.0 - angleOuter1_deg) * 2.0;
      double angleInner2 = (180.0 - 90.0 - angleOuter2_deg) * 2.0;

      //return the middle value of the two
      return (angleInner1 + angleInner2) / 2.0;
      break;
    case fromRight:
      //calculate middle acute angle
      tdoa = soundDetectionTimes[1] - soundDetectionTimes[2];
      /*tdoaDistance = SPEED_OF_SOUND * tdoa;
      angle_for_cos = double(tdoaDistance) / double(MIC_DISTANCE);
      angle_rad = acos(angle_for_cos); //returns angle in radians
      angle_deg = angle_rad * 180.0 / M_PI; //returns angle in degrees*/
      Serial.print("tdoa: "); Serial.println(tdoa);
      tdoaDistance = SPEED_OF_SOUND * tdoa; //returns μm/μs * μs = μm
      Serial.print("tdoaDistance: "); Serial.println(tdoaDistance);
      angle_for_cos = double(tdoaDistance) / double(MIC_DISTANCE);
      Serial.print("angle_for_cos: "); Serial.println(angle_for_cos);
      angle_rad = acos(angle_for_cos); //returns angle in radians
      Serial.print("angle_rad: "); Serial.println(angle_rad);
      angle_deg = angle_rad * 180.0 / M_PI; //returns angle in degrees
      Serial.print("angle_deg: "); Serial.println(angle_deg);
      return angle_deg;
      break;
    default:
      break;
  }
  //if all else fails...
  Serial.println("all else failed...");
  return 90.0;
}
