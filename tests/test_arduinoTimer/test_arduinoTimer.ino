// AVR Timer CTC Interrupts Calculator
// v. 8
// http://www.arduinoslovakia.eu/application/timer-calculator
// Microcontroller: ATmega2560
// Created: 2026-05-15T14:48:25.870Z

const int PIN_TIMER = 9;
bool state = false;

void setupTimer1() {
  noInterrupts();
  // Clear registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 2 changes at 20kHz = 40kHz
  // 40000 Hz (16000000/((49+1)*8))
  OCR1A = 49;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 8
  TCCR1B |= (1 << CS11);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

void setup() {
  pinMode(PIN_TIMER, OUTPUT);
  setupTimer1();
}

void loop() {
}

ISR(TIMER1_COMPA_vect) {
  if(state) {
    digitalWrite(PIN_TIMER, HIGH);
    state = false;
  } else if(!state) {
    digitalWrite(PIN_TIMER, LOW);
    state = true;
  }
}