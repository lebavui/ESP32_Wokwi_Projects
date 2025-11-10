#include <Arduino.h>

void Task1(void *pvParameters);
void Task2(void *pvParameters);

void setup() {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  xTaskCreate(Task1, "Task1", 1024, NULL, 1, NULL);
  xTaskCreate(Task2, "Task2", 1024, NULL, 1, NULL);
}

void loop() { }

void Task1(void *pvParameters) {
  for(;;) {
    digitalWrite(4, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(4, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void Task2(void *pvParameters) {
  for(;;) {
    digitalWrite(5, HIGH);
    vTaskDelay(800 / portTICK_PERIOD_MS);
    digitalWrite(5, LOW);
    vTaskDelay(800 / portTICK_PERIOD_MS);
  }
}