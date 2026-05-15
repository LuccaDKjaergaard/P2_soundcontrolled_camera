#include <math.h>
#include <Servo.h>
#include <krnl.h>

#define PIN_MIC1 A0
#define PIN_MIC2 A2
#define PIN_MIC3 A3

const unsigned int SPEED_OF_SOUND = 343; //m/s = mm/ms = microm/micros
const unsigned int MIC_DISTANCE = 100000; //micro m
const unsigned int SOUND_THRESHOLD = 700; //analog value for sound detection
unsigned long soundDetectionTimes[3];

Servo servo; //initialise servo motor

struct k_t *pTask1, *pTask2, *pTask3, *pTask4; //create task pointers
//struct k_t *sem1, *sem2, *sem3, *sem4; //create semaphore pointers
const int STACK = 150; //stack to be used by tasks
unsigned char taskStack1[STACK], taskStack2[STACK], taskStack3[STACK], taskStack4[STACK];

void task1() {
  while(1) {
    if(analogRead(PIN_MIC1) > SOUND_THRESHOLD) {
      //keep CPU
      //soundDetectionTimes[0] = micros();
      //set mic1 bool
      //give CPU back
      
      //Serial.print("Mic 1: "); Serial.println(micros());
    }
  }
}

void task2() {
  while(1) {
    if(analogRead(PIN_MIC2) > SOUND_THRESHOLD) {
      Serial.print("Mic 2: "); Serial.println(micros());
    }
  }
}

void task3() {
  while(1) {
    if(analogRead(PIN_MIC3) > SOUND_THRESHOLD) {
      Serial.print("Mic 3: "); Serial.println(micros());
    }
  }
}

//task for doing Serial stuffs
void taskSerial() {

}

void setup() {
  Serial.begin(115200);
  while(!Serial);

  pinMode(PIN_MIC1, INPUT);
  pinMode(PIN_MIC2, INPUT);
  pinMode(PIN_MIC3, INPUT);
  
  servo.attach(9);
  servo.write(90); //place servo in middle

  k_init(4, 0, 0);
  pTask1 = k_crt_task(task1, 10, taskStack1, STACK);
  pTask2 = k_crt_task(task2, 10, taskStack2, STACK);
  pTask2 = k_crt_task(task3, 10, taskStack3, STACK);
  pTask2 = k_crt_task(taskSerial, 12, taskStack4, STACK);

  int error;
  error = k_start(); //should never return from here
  Serial.print("An error occured: "); Serial.println(error);
  while(1);
  /*pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(18, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), pin2, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), pin3, FALLING);
  attachInterrupt(digitalPinToInterrupt(18), pin18, FALLING);*/
}

/*void pin2() {
  Serial.print("Pin 2: "); Serial.println(millis());
}

void pin3() {
  Serial.print("Pin 3: "); Serial.println(millis());
}

void pin18() {
  Serial.print("Pin 18: "); Serial.println(millis());
}
*/
void loop() {
}
