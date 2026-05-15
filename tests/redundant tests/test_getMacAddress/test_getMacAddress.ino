//#include <WiFi.h>
//#include <esp_wifi.h> //was included in example but code works without ¯\_(ツ)_/¯

void setup() {
  Serial.begin(115200);
  while(!Serial);
  
  //WiFi.mode(WIFI_STA); //initialise WiFi library
  //delay(1000); //wait for initialising of WiFi library

  Serial.print("MAC-address: ");
  //Serial.println(WiFi.macAddress());
  Serial.println(ESP.getEfuseMac());
}

void loop() {

}
