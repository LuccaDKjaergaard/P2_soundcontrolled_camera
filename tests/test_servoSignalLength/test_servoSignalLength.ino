const int PIN_TIMER = 3;
const int LOOPS = 100;

int valueRight = 400;
int valueLeft = 2400;
int valueMiddle = ((valueRight + valueLeft) / 2);

void setup() {}

void setup1() {
  pinMode(PIN_TIMER, OUTPUT);
}

void loop() {}

void loop1() {
  for(int i = 0; i < LOOPS; i++) {
    digitalWrite(PIN_TIMER, HIGH);
    delayMicroseconds(valueRight);
    digitalWrite(PIN_TIMER, LOW);
    delay(17);
    delayMicroseconds(3000 - valueRight);
  }
  
  for(int i = 0; i < LOOPS; i++) {
    digitalWrite(PIN_TIMER, HIGH);
    delayMicroseconds(valueMiddle);
    digitalWrite(PIN_TIMER, LOW);
    delay(17);
    delayMicroseconds(3000 - valueMiddle);
  }

  for(int i = 0; i < LOOPS; i++) {
    digitalWrite(PIN_TIMER, HIGH);
    delayMicroseconds(valueLeft);
    digitalWrite(PIN_TIMER, LOW);
    delay(17);
    delayMicroseconds(3000 - valueLeft);
  }
}
