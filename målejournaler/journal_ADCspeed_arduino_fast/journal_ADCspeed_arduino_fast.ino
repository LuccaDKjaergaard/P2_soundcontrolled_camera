//Arduino MEGA datasheet:
//https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf

const int LOOPS = 1000;
const int ANALOG_PORTS = 8; //datasheet pp. 217-218
int array[ANALOG_PORTS];

void setup() {
  Serial.begin(115200);
  ADCSRA |= B10000000; //enable ADC
}

void loop() {
  testADCfast();
}

void testADCfast() {
  unsigned long timeStart, timeFinish;
  timeStart = millis();
  for (int i = 0; i < LOOPS; i++) {
    //https://forum.arduino.cc/t/analog-reading-whit-registers/922403
    //datasheet pp. 217-218

    ADMUX &= B11110000; //clear out channel bits
    ADMUX |= B00000000; //A0;
    ADCSRA |= B11000000; //start ADC conversion
    while(bit_is_set(ADCSRA, ADSC)); //wait for end of conversion
    array[0] = (ADCH << 8) | ADCL; //combine ADC9-8 and ADC7-0

    ADMUX &= B11110000; //clear out channel bits
    ADMUX |= B00000001; //A1
    ADCSRA |= B11000000; //start ADC conversion
    //while(bit_is_set(ADCSRA, ADSC)); //wait for end of conversion
    array[1] = (ADCH << 8) | ADCL; //combine ADC9-8 and ADC7-0

    ADMUX &= B11110000; //clear out channel bits
    ADMUX |= B00000010; //A2
    ADCSRA |= B11000000; //start ADC conversion
    //while(bit_is_set(ADCSRA, ADSC)); //wait for end of conversion
    array[2] = (ADCH << 8) | ADCL; //combine ADC9-8 and ADC7-0

    ADMUX &= B11110000; //clear out channel bits
    ADMUX |= B00000011; //A3
    ADCSRA |= B11000000; //start ADC conversion
    //while(bit_is_set(ADCSRA, ADSC)); //wait for end of conversion
    array[3] = (ADCH << 8) | ADCL; //combine ADC9-8 and ADC7-0

    ADMUX &= B11110000; //clear out channel bits
    ADMUX |= B00000100; //A4
    ADCSRA |= B11000000; //start ADC conversion
    //while(bit_is_set(ADCSRA, ADSC)); //wait for end of conversion
    array[4] = (ADCH << 8) | ADCL; //combine ADC9-8 and ADC7-0

    ADMUX &= B11110000; //clear out channel bits
    ADMUX |= B00000101; //A5
    ADCSRA |= B11000000; //start ADC conversion
    //while(bit_is_set(ADCSRA, ADSC)); //wait for end of conversion
    array[5] = (ADCH << 8) | ADCL; //combine ADC9-8 and ADC7-0

    ADMUX &= B11110000; //clear out channel bits
    ADMUX |= B00000110; //A6
    ADCSRA |= B11000000; //start ADC conversion
    //while(bit_is_set(ADCSRA, ADSC)); //wait for end of conversion
    array[6] = (ADCH << 8) | ADCL; //combine ADC9-8 and ADC7-0

    ADMUX &= B11110000; //clear out channel bits
    ADMUX |= B00000111; //A7
    ADCSRA |= B11000000; //start ADC conversion
    //while(bit_is_set(ADCSRA, ADSC)); //wait for end of conversion
    array[7] = (ADCH << 8) | ADCL; //combine ADC9-8 and ADC7-0
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