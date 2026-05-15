const int LOOPS = 1000;
const int NUM_DIGITAL_PORTS = 23; //Raspberry Pi Pico-2W
bool array[NUM_DIGITAL_PORTS];
const int DIGITAL_PINS[NUM_DIGITAL_PORTS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                                             12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};

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
