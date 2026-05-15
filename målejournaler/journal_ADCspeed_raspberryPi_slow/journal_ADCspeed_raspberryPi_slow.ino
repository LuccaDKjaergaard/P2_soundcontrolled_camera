const int LOOPS = 1000;
const int ANALOG_PORTS = 3; //Raspberry Pi Pico2W
int array[ANALOG_PORTS];

void setup() {
  Serial.begin(115200);
  while(!Serial); //wait for serial

  pinMode(26, INPUT);
  pinMode(27, INPUT);
  pinMode(28, INPUT);
}

void loop() {
  testAnalogRead();
}

void testAnalogRead() {
  unsigned long timeStart, timeFinish;
  timeStart = millis();
  for(int i = 0; i < LOOPS; i++) {
    array[0] = analogRead(26); //A0
    array[1] = analogRead(27); //A1
    array[2] = analogRead(28); //A2
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
