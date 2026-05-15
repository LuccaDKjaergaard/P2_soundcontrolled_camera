#include <Servo.h>
#include <math.h>

const int MIC_THRESHOLD = 700;
//const int RECORDING_DURATION = 1;  // Duration of the recording in seconds
const int NUM_SAMPLES = 30;  // Number of samples to record
//const int SAMPLING_RATE = 50;  // Sampling rate of the recording in Hz
//const int NUM_CHANNELS = 1;  // Number of channels (1 for mono, 2 for stereo)
//const int BIT_DEPTH = 16;  // Bit depth of the recording (8, 16, 24, or 32)

Servo servo; //initialise servo motor

#define mic1_pin A2
#define mic2_pin A1
#define mic3_pin A0

int mic1_levels[NUM_SAMPLES];  // Array for storing the power levels of microphone 1
int mic2_levels[NUM_SAMPLES];  // Array for storing the power levels of microphone 2
int mic3_levels[NUM_SAMPLES];  // Array for storing the power levels of microphone 3
int mic_baselines[3];  

void setup() {
  Serial.begin(115200);
  while(!Serial);

  servo.attach(9);
  pinMode(mic1_pin, INPUT);
  pinMode(mic2_pin, INPUT);
  pinMode(mic3_pin, INPUT);

  Serial.println("Waiting for circuit to power up...");
  delay(1000);

  
  Serial.println("Calculating mic baselines...");
  const int NUM_SAMPLES = 1000;
  int mic1_baselines[NUM_SAMPLES];
  int mic2_baselines[NUM_SAMPLES];
  int mic3_baselines[NUM_SAMPLES];
  for(int i = 0; i < NUM_SAMPLES; i++) { //read a lot of samples
    mic1_baselines[i] = analogRead(mic1_pin);
    mic2_baselines[i] = analogRead(mic2_pin);
    mic3_baselines[i] = analogRead(mic3_pin);
  }
  unsigned long mic1_total = 0;
  unsigned long mic2_total = 0;
  unsigned long mic3_total = 0;
  for(int i = 0; i < NUM_SAMPLES; i++) { //add all samples together
    mic1_total += mic1_baselines[i];
    mic2_total += mic2_baselines[i];
    mic3_total += mic3_baselines[i];
  }
  //calculate averages and write in global array
  mic_baselines[0] = mic1_total / NUM_SAMPLES;
  mic_baselines[1] = mic2_total / NUM_SAMPLES;
  mic_baselines[2] = mic3_total / NUM_SAMPLES;
  Serial.println("Done calculating mic baselines!");
  Serial.println("Baselines: ");
  for(int i = 0; i < 3; i++) {
    Serial.print(mic_baselines[i]);
    Serial.print(" , ");
  }
  
}

void loop() {
  // Read the values from the microphones
  int servo_pos;
  if(analogRead(mic1_pin) > MIC_THRESHOLD || analogRead(mic2_pin) > MIC_THRESHOLD || analogRead(mic3_pin) > MIC_THRESHOLD) {
    for (int i = 0; i < NUM_SAMPLES; i++) { //mic_baselines eliminates offset in hardware
      mic1_levels[i] = analogRead(mic1_pin) - mic_baselines[0];
      mic2_levels[i] = analogRead(mic2_pin) - mic_baselines[1];
      mic3_levels[i] = analogRead(mic3_pin) - mic_baselines[2];
    }
 
    int mic1_avg = 0;
    int mic2_avg = 0;
    int mic3_avg = 0;
    /*int max1 = 0;
    int max2 = 0;
    int max3 = 0;*/
    for (int i = 0; i < NUM_SAMPLES; i++) {
      mic1_avg += mic1_levels[i];
      mic2_avg += mic2_levels[i];
      mic3_avg += mic3_levels[i];
          
      /*max1 = max(mic1_levels[i], max1);
      max2 = max(mic2_levels[i], max2);
      max3 = max(mic3_levels[i], max3);*/
    }
    //get averages:
    mic1_avg /= NUM_SAMPLES;
    mic2_avg /= NUM_SAMPLES;
    mic3_avg /= NUM_SAMPLES;

    // Serial.print("\nmic 1:");
    // Serial.print(max1);
    // Serial.print("\nmic 2:");
    // Serial.print(max2);
    // Serial.print("\nmic 3:");   
    // Serial.print(max3);

    //define vectors for each microphone in polar form
    double mic1_vector[] = {180, mic1_avg};
    double mic2_vector[] = {90, mic2_avg};
    double mic3_vector[] = {0, mic3_avg};

    //define vectors for each microphone in polar form
    /*double mic1_vector[] = {180, max1};
    double mic2_vector[] = {90, max2};
    double mic3_vector[] = {0, max3};*/

    //convert the vectors to rectangular form
    //cos() and sin() need the angle in radians
    double mic1_rect[] = {mic1_vector[1] * cos(mic1_vector[0] * M_PI / 180), mic1_vector[1] * sin(mic1_vector[0] * M_PI / 180)};
    double mic2_rect[] = {mic2_vector[1] * cos(mic2_vector[0] * M_PI / 180), mic2_vector[1] * sin(mic2_vector[0] * M_PI / 180)};
    double mic3_rect[] = {mic3_vector[1] * cos(mic3_vector[0] * M_PI / 180), mic3_vector[1] * sin(mic3_vector[0] * M_PI / 180)};

    //add the vectors
    double result_x = mic1_rect[0] + mic2_rect[0] + mic3_rect[0];
    double result_y = mic1_rect[1] + mic2_rect[1] + mic3_rect[1];

    Serial.print("\nX:");
    Serial.print(result_x);
    Serial.print("\nY:");
    Serial.print(result_y);
    Serial.print("\n");

    //convert the result back to polar form:
    double result_magnitude = sqrt(result_x * result_x + result_y * result_y);
    double result_angle = atan2(result_y, result_x) * 180 / M_PI; //returns result in degrees between -180 and 180

    Serial.print("\nX:");
    Serial.print(result_magnitude);
    Serial.print("\nAngle:");
    Serial.print(map(result_angle, -180, 180, 0, 180));
    Serial.print("\n");

    //int servo_pos = map(result_angle, -180, 180, 0, 180); //map the angle to a servo position
    servo.write(result_angle); //move the servo to the calculated position
    delay(2000); //wait for signal to settle after servo movement
  }
}