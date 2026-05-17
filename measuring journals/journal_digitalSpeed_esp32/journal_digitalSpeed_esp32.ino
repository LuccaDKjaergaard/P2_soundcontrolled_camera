const int LOOPS = 1000;
const int NUM_DIGITAL_PORTS = 26; //ESP-WROOM-32
int array[NUM_DIGITAL_PORTS];
const int DIGITAL_PINS[NUM_DIGITAL_PORTS] = {36, 39, 34, 35, 32, 33, 25, 26, 27, 14, 12, 13,
                                             23, 22, 1, 3, 21, 19, 18, 5, 17, 16, 4, 0, 2, 15};

void setup() {
  Serial.begin(115200);
  while(!Serial); //wait for serial
  Serial.println("debug");
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
