#define __FREERTOS 1
#include <FreeRTOS.h>
#include <task.h>

void TaskBlink(void *pvParameters) {
  pinMode(LED_BUILTIN, OUTPUT);
  while (1) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void TaskSerial(void *pvParameters) {
  while (1) {
    Serial.println("Hello from FreeRTOS!");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  xTaskCreate(TaskBlink, "Blink", 512, NULL, 1, NULL);
  xTaskCreate(TaskSerial, "Serial", 512, NULL, 1, NULL);

  vTaskStartScheduler();
}

void loop() {
  // Empty — FreeRTOS takes over
}
