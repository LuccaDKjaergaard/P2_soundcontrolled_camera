int valueRight = 400;
int valueMiddle = 1400;
int valueLeft = 2400;
int servoPosition = valueMiddle;

void setup() {
  Serial.begin(115200);
}

void setup1() {
  pinMode(3, OUTPUT);
}

void loop() {
  int num = random(0, 180);
  servoPosition = map(num, 0, 180, 400, 2400);
  Serial.println(num);
  delay(1000);
}

void loop1() {
  digitalWrite(3, HIGH);
  delayMicroseconds(servoPosition);
  digitalWrite(3, LOW);
  delay(17);
  delayMicroseconds(3000 - servoPosition);
}
