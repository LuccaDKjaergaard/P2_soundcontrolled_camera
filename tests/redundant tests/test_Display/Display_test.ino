#include <TFT_eSPI.h> //remember to edit library files
TFT_eSPI tft = TFT_eSPI();

void setup() {
  InitDisplay();
  DrawDisplay();
}

void loop() {

}

//initialise TTGO LilyGo T-display
void InitDisplay() {
  tft.init();
  tft.setRotation(1); //landscape mode
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2); //scale text
}

void DrawDisplay() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0);
  tft.println("TEST TEXT:");

  tft.setCursor(0, 30);
  tft.print("This is line 1.");

  tft.setCursor(0, 60);
  tft.print("This is line 2.");

  tft.setCursor(0, 90);
  tft.print("This is line 3.");

  tft.setCursor(0, 120);
  tft.print("This is line 4.");
}
