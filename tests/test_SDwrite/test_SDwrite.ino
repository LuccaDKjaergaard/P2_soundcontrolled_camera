#include <SD.h>
//connect SD module to MCU's default MOSI, MISO, SCLK
const int PIN_CS = 53; //chipselect
File file; //initialise file
char fileTitle = "file.txt";

unsigned int counter = 0;

void setup() {
  Serial.begin(115200);
  while(!Serial); //wait for Serial


  //do pinmodes
  //attachinterrupt
  
  InitSD(fileTitle);
  InitKrnl();
}

void loop() {
  /*
  while(interruptNotGotten) {
    for(int i = 0; i < sizeof(unsigned int); i++) {

      if(counter)
    }
  }
  */

  //if(data.available)
  //write to SD
}

void writeToSD(unsigned long incomingTime, unsigned long incomingData) {
  file = SD.open(fileTitle, FILE_WRITE);

  file.print(incomingTime);
  file.print(", ");
  file.println(incomingData);

}

void InitSD(char title) {
  Serial.println("Initialising SD card... ");

  if(SD.begin(PIN_CS)) {
    Serial.println("initialisation done.");
  } else {
    Serial.println("initialisation failed.");
  }

  //create file by opening and closing
  file = SD.open(title, FILE_WRITE);
  file.close();

  if(SD.exists(title)) {
    Serial.print(title); Serial.println(" exists.");
  } else {
    Serial.print(title); Serial.println(" doesn't exist.");
  }
}

void InitKrnl() {
  
}
