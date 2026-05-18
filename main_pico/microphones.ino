void InitMicrophones() {
  micLeft.pin = 26; //A0
  micMiddle.pin = 27; //A1
  micRight.pin = 28; //A2

  pinMode(micLeft.pin, INPUT);
  pinMode(micMiddle.pin, INPUT);
  pinMode(micRight.pin, INPUT);
}

int CalculateSoundAngle() {
  const double SPEED_OF_SOUND = 343.0; //m/s = mm/ms = microm/micros
  const double MIC_DISTANCE = 160000.0; //micro m (= 15 cm)
  double tdoa, tdoaDistance;
  double angleRadians, angleDegrees;

  switch(soundState) {
    case fromLeft:
      tdoa = micRight.detectedTime - micMiddle.detectedTime;
      Serial.print("tdoa: "); Serial.println(tdoa);
      tdoaDistance = SPEED_OF_SOUND * tdoa;
      Serial.print("tdoaDistance: "); Serial.println(tdoaDistance);
      if(tdoaDistance > MIC_DISTANCE) {
        Serial.println("tdoaDistance is too large.");
        return 135;
      }

      angleRadians = acos(tdoaDistance / MIC_DISTANCE); //if tdoaDistance > MIC_DISTANCE then angleRadians = inf
      Serial.print("angleRadians: "); Serial.println(angleRadians); //should be in [0, pi]
      angleDegrees = angleRadians * 180.0 / M_PI;
      return map(int(angleDegrees), 0, 180, 180, 0);
      break;
    case fromRight:
      tdoa = micLeft.detectedTime - micMiddle.detectedTime;
      Serial.print("tdoa: "); Serial.println(tdoa);
      tdoaDistance = SPEED_OF_SOUND * tdoa;
      Serial.print("tdoaDistance: "); Serial.println(tdoaDistance);
      if(tdoaDistance > MIC_DISTANCE) {
        Serial.println("tdoaDistance is too large.");
        return 45;
      }

      angleRadians = acos(tdoaDistance / MIC_DISTANCE); //if tdoaDistance > MIC_DISTANCE then angleRadians = inf
      Serial.print("angleRadians: "); Serial.println(angleRadians); //should be in [0, pi]
      angleDegrees = angleRadians * 180.0 / M_PI;
      return int(angleDegrees);
      break;
    case unknown:
    default:
      break;
  }
  //if all else fails...
  return 90;
}