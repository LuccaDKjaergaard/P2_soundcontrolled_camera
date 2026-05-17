const int LOOPS = 1000;
const int ANALOG_PORTS = 16; //ESP-WROOM-32
const int ANALOG_PINS[ANALOG_PORTS] = {36, 39, 32, 33, 34, 35, 25, //ADC1_(0, 3-8)
                                       4, 0, 2, 15, 13, 12, 14, 27, 26}; //ADC2_(0-7, 9)
int array[ANALOG_PORTS];

void setup() {
  Serial.begin(115200);
  while(!Serial); //wait for serial

  for(int i = 0; i < ANALOG_PORTS; i++) {
    pinMode(ANALOG_PINS[i], INPUT);
  }
}

void loop() {
  testAnalogRead();
}

void testAnalogRead() {
  unsigned long timeStart, timeFinish;
  timeStart = millis();
  for(int i = 0; i < LOOPS; i++) {
    for(int j = 0; j < ANALOG_PORTS; j++) {
      array[j] = analogRead(ANALOG_PINS[j]);
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
  Serial.print("Analog reading time: ");
  Serial.print( (timeFinishFloat - timeStartFloat) / (float(LOOPS) * float(ANALOG_PORTS)), 4); //print with 4 decimals
  Serial.println(" msec.");
}
