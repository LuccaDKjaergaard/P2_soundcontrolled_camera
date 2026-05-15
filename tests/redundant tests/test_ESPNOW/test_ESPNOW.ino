#include <WiFi.h>
//#include <esp_wifi.h> //was included in example but code works without ¯\_(ツ)_/¯
#include <ESP32_NOW.h>
#include <ESP32_NOW_Serial.h>

uint8_t slaveMAC[6] = {48, 174, 164, 123, 70, 204};

struct wirelessDataTp {
  int integer = 0;
  float floating = 0.0;
  char character;
};
struct wirelessDataTp temporaryIncomingData;

esp_now_peer_info_t slaveInfo;

void setup() {
  Serial.begin(115200);
  while(!Serial); //wait for Serial
  
  //initialisation:
  WiFi.mode(WIFI_STA); //initialise WiFi library
  delay(1000); //wait for initialising of WiFi library
  InitESPNOW(); //must be initialised after WiFi library
}

void loop() {

}

void InitESPNOW() {
  Serial.println("Waiting for ESP_NOW...");
  while(!(esp_now_init() == ESP_OK)); //wait for initialisation of library

  //connect to peer:
  for(int i = 0; i < 6; i++) {
    slaveInfo.peer_addr[i] = slaveMAC[i];
  }
  slaveInfo.channel = 0; //use default channel
  slaveInfo.encrypt = false; //can be changed later

  if(esp_now_add_peer(&slaveInfo) == ESP_OK) {
    Serial.println("Peer added successfully.");
  } else {
    Serial.print("Failed to add peer. Error code: ");
    Serial.println(esp_now_add_peer(&slaveInfo));
  }
}
