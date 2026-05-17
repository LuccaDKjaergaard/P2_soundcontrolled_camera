// AVR Timer CTC Interrupts Calculator
// v. 8
// http://www.arduinoslovakia.eu/application/timer-calculator
// Microcontroller: ATmega2560
// Created: 2026-05-15T14:48:25.870Z

const int PIN_TIMER = 9;
bool state = false;

void setupTimer1() {
  noInterrupts();
  //Clear registers:
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  //2 changes at 40kHz = 20kHz
  //40000 Hz (16000000/((49+1)*8))
  OCR1A = 48; //Adjusted to be closer to 40kHz output
  TCCR1B |= (1 << WGM12); //CTC
  TCCR1B |= (1 << CS11); //Prescaler 8
  TIMSK1 |= (1 << OCIE1A); //Output Compare Interrupt Enable Match A 
  interrupts();
}

void setup() {
  pinMode(PIN_TIMER, OUTPUT);
  setupTimer1();
}

void loop() {}

ISR(TIMER1_COMPA_vect) {
  digitalWrite(PIN_TIMER, state);
  state = state^1;
}