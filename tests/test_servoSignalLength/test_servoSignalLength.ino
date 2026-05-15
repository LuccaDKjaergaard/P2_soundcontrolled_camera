int valueRight = 400;
int valueMiddle = 1400;
int valueLeft = 2400;

void setup() {}

void setup1() {
  pinMode(3, OUTPUT);
}

void loop() {}

void loop1() {
  for(int i = 0; i < 100; i++) {
    digitalWrite(3, HIGH);
    delayMicroseconds(valueRight);
    digitalWrite(3, LOW);
    delay(18);
    delayMicroseconds(2000 - valueRight);
  }
  
  for(int i = 0; i < 100; i++) {
    digitalWrite(3, HIGH);
    delayMicroseconds(valueMiddle);
    digitalWrite(3, LOW);
    delay(18);
    delayMicroseconds(2000 - valueMiddle);
  }

  
  for(int i = 0; i < 100; i++) {
    digitalWrite(3, HIGH);
    delayMicroseconds(valueLeft);
    digitalWrite(3, LOW);
    delay(17);
    delayMicroseconds(3000 - valueLeft);
  }
}
