#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

static int shared_counter = 0;
SemaphoreHandle_t mutex;

// Task: Increment shared counter safely
void incTask(void *parameters) {
    int local_var;

    while (1) {
        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        local_var = shared_counter;
        local_var++;
        vTaskDelay(pdMS_TO_TICKS(100));  // simulate work
        shared_counter = local_var;

        Serial.printf("Counter = %d\n", shared_counter);

        xSemaphoreGive(mutex);
        }

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

// Task: Read user input to set counter start value
void inputTask(void *parameters) {
    while (1) {
        if (Serial.available() > 0) {
        int val = Serial.parseInt();

        if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
            shared_counter = val;
            Serial.printf("Counter reset to %d\n", shared_counter);
            xSemaphoreGive(mutex);
        }

        // Clear remaining input
        while (Serial.available()) Serial.read();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void setup() {
    Serial.begin(115200);
    vTaskDelay(pdMS_TO_TICKS(1000));
    Serial.println("\n--- FreeRTOS Mutex Demo ---");
    Serial.println("Enter a number to reset the counter:");

    // Create mutex
    mutex = xSemaphoreCreateMutex();
    if (mutex == NULL) {
        Serial.println("Failed to create mutex!");
        while (1);
    }

    // Create tasks
    xTaskCreate(incTask, "Increment Task 1", 2048, NULL, 1, NULL);
    xTaskCreate(incTask, "Increment Task 2", 2048, NULL, 1, NULL);
    xTaskCreate(inputTask, "Input Task", 2048, NULL, 1, NULL);

    vTaskDelete(NULL);  // end setup task
}

void loop() {
  // Not used
}
