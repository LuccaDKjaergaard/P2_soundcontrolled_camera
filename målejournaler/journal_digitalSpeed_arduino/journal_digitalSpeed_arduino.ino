const int LOOPS = 1000;
const int NUM_DIGITAL_PORTS = 44; //12 for Arduino UNO, 44 for Arduino MEGA
bool array[NUM_DIGITAL_PORTS];
//const int DIGITAL_PINS[NUM_DIGITAL_PORTS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
//uncomment the following for Arduino MEGA:
const int DIGITAL_PINS[NUM_DIGITAL_PORTS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
                                         22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
                                         32, 33, 34, 35, 36, 37, 38, 39, 40, 41,
                                         42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53};

void setup() {
  Serial.begin(115200);
  while(!Serial); //wait for serial

  for(int i = 0; i < NUM_DIGITAL_PORTS; i++) {
    pinMode(DIGITAL_PINS[i], INPUT);
  }
}

void loop() {
  testDigitalRead();
}

void testDigitalRead() {
  unsigned long timeStart, timeFinish;
  timeStart = millis();
  for(int i = 0; i < LOOPS; i++) {
    for(int j = 0; j < NUM_DIGITAL_PORTS; j++) {
      array[j] = digitalRead(DIGITAL_PINS[j]);
    }
  }
  timeFinish = millis();
  Serial.print("Total time: ");
  Serial.print(timeFinish - timeStart);
  Serial.print(" msec. ");

  float timeStartFloat, timeFinishFloat;
  timeStartFloat = timeStart;
  timeFinishFloat = timeFinish;
  Serial.print("Loop time: ");
  Serial.print( (timeFinishFloat - timeStartFloat) / (float(LOOPS) ), 4); //print with 4 decimals
  Serial.print(" msec. ");
  Serial.print("Digital reading time: ");
  Serial.print( (timeFinishFloat - timeStartFloat) / (float(LOOPS) * float(NUM_DIGITAL_PORTS)), 4); //print with 4 decimals
  Serial.println(" msec.");
}
