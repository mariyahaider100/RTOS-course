## 🔒 FreeRTOS Mutex Example (ESP32)

### 🎯 Objective

1. Learn how a **mutex (mutual exclusion)** mechanism works in FreeRTOS.  
2. Accept an integer input from the Serial Monitor to set the **initial value of a shared counter**.  
3. Safely update and print the counter using a mutex to prevent data conflicts between multiple tasks.

---

### 🧠 Overview

A **mutex** allows only one task to access shared resources at a time.  
When one task “takes” the mutex, all others trying to access the same resource must wait until it is “released.”  
This synchronization prevents inconsistent or corrupted data in concurrent systems.

---

### 🧩 Complete Code

```cpp
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

static int shared_counter = 0;
SemaphoreHandle_t mutex;

// Task: Safely increment the shared counter
void incTask(void *parameters) {
  int local_var;

  while (1) {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
      local_var = shared_counter;
      local_var++;
      vTaskDelay(pdMS_TO_TICKS(100));  // Simulate some processing delay
      shared_counter = local_var;

      Serial.printf("Counter = %d\n", shared_counter);

      xSemaphoreGive(mutex);
    }

    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

// Task: Read Serial input and reset counter
void inputTask(void *parameters) {
  while (1) {
    if (Serial.available() > 0) {
      int val = Serial.parseInt();

      if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
        shared_counter = val;
        Serial.printf("Counter reset to %d\n", shared_counter);
        xSemaphoreGive(mutex);
      }

      // Clear any leftover input
      while (Serial.available()) Serial.read();
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(1000));
  Serial.println("\n--- FreeRTOS Mutex Example ---");
  Serial.println("Type a number in Serial Monitor to reset the counter:");

  // Create mutex
  mutex = xSemaphoreCreateMutex();
  if (mutex == NULL) {
    Serial.println("Mutex creation failed!");
    while (1);
  }

  // Create tasks
  xTaskCreate(incTask, "Increment Task 1", 2048, NULL, 1, NULL);
  xTaskCreate(incTask, "Increment Task 2", 2048, NULL, 1, NULL);
  xTaskCreate(inputTask, "Input Task", 2048, NULL, 1, NULL);

  vTaskDelete(NULL);  // Delete setup task
}

void loop() {
  // Unused — all logic handled in FreeRTOS tasks
}
