## ⏱️ FreeRTOS Software Timer Example (ESP32)

### 🎯 Objective

1. Learn how **FreeRTOS software timers** function — covering both *one-shot* and *auto-reload* modes.  
2. Implement **two timers**:
   - **Timer 1:** Toggle the onboard LED every **1 second**.  
   - **Timer 2:** Read **ADC (A0)** and print to Serial every **500 ms**.  

---

### 🧠 Overview

FreeRTOS software timers allow tasks to run periodically or once after a delay — without occupying the CPU with delay loops.  
They operate within the **timer service task**, enabling multitasking and efficient resource use.

---

### 🧩 Complete Code

```cpp
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

// Built-in LED and analog input pin
const int LED_PIN = 2;
const int ADC_PIN = A0;

// Timer handles
static TimerHandle_t led_timer = NULL;
static TimerHandle_t adc_timer = NULL;

// Shared callback function
void myTimerCallback(TimerHandle_t xTimer) {
  uint32_t timer_id = (uint32_t)pvTimerGetTimerID(xTimer);

  // LED toggle timer
  if (timer_id == 0) {
    static bool led_state = false;
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state);
    Serial.printf("[LED Timer] LED %s\n", led_state ? "ON" : "OFF");
  }

  // ADC read timer
  if (timer_id == 1) {
    int value = analogRead(ADC_PIN);
    Serial.printf("[ADC Timer] ADC value: %d\n", value);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ADC_PIN, INPUT);
  vTaskDelay(pdMS_TO_TICKS(1000));

  Serial.println("\n--- FreeRTOS Software Timer Example ---");

  // Create timers
  led_timer = xTimerCreate(
      "LED Timer",
      pdMS_TO_TICKS(1000),  // 1s interval
      pdTRUE,               // auto-reload
      (void*)0,
      myTimerCallback);

  adc_timer = xTimerCreate(
      "ADC Timer",
      pdMS_TO_TICKS(500),   // 0.5s interval
      pdTRUE,               // auto-reload
      (void*)1,
      myTimerCallback);

  if (led_timer == NULL || adc_timer == NULL) {
    Serial.println("Timer creation failed!");
    while (1);
  }

  // Start both timers
  xTimerStart(led_timer, portMAX_DELAY);
  xTimerStart(adc_timer, portMAX_DELAY);
}

void loop() {
  // Empty loop — timers handle everything
}
