const int LOOPS = 1000;
const int ANALOG_PORTS = 6; //6 for Arduino UNO, 16 for Arduino MEGA
int array[ANALOG_PORTS];

void setup() {
  Serial.begin(115200);
  while(!Serial); //wait for serial

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  //uncomment the following for Arduino MEGA:
  /*pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);
  pinMode(A15, INPUT);*/
}

void loop() {
  testAnalogRead();
}

void testAnalogRead() {
  unsigned long timeStart, timeFinish;
  timeStart = millis();
  for(int i = 0; i < LOOPS; i++) {
    array[0] = analogRead(A0);
    array[1] = analogRead(A1);
    array[2] = analogRead(A2);
    array[3] = analogRead(A3);
    array[4] = analogRead(A4);
    array[5] = analogRead(A5);
    //uncomment the following for Arduino MEGA:
    /*array[6] = analogRead(A6);
    array[7] = analogRead(A7);
    array[8] = analogRead(A8);
    array[9] = analogRead(A9);
    array[10] = analogRead(A10);
    array[11] = analogRead(A11);
    array[12] = analogRead(A12);
    array[13] = analogRead(A13);
    array[14] = analogRead(A14);
    array[15] = analogRead(A15);*/
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
