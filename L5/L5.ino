#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

// Onboard LED pin (ESP32 built-in)
const int LED_PIN = 2;
const int ADC_PIN = A0;

// Timer handles
static TimerHandle_t led_timer = NULL;
static TimerHandle_t adc_timer = NULL;

// Callback for both timers
void myTimerCallback(TimerHandle_t xTimer) {
    uint32_t timer_id = (uint32_t)pvTimerGetTimerID(xTimer);

    // LED blink timer
    if (timer_id == 0) {
        static bool led_state = false;
        led_state = !led_state;
        digitalWrite(LED_PIN, led_state);
        Serial.printf("[LED Timer] LED %s\n", led_state ? "ON" : "OFF");
    }

    // ADC send timer
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

    Serial.println("\n--- FreeRTOS Software Timer Demo ---");

    // Create timers
    led_timer = xTimerCreate("LED Timer",
                            pdMS_TO_TICKS(1000),   // 1s
                            pdTRUE,                // auto-reload
                            (void*)0,
                            myTimerCallback);

    adc_timer = xTimerCreate("ADC Timer",
                            pdMS_TO_TICKS(500),    // 0.5s
                            pdTRUE,                // auto-reload
                            (void*)1,
                            myTimerCallback);

    if (led_timer == NULL || adc_timer == NULL) {
        Serial.println("Failed to create timers!");
        while (1);
    }

    // Start both timers
    xTimerStart(led_timer, portMAX_DELAY);
    xTimerStart(adc_timer, portMAX_DELAY);
}

void loop() {
  // Not used — timers handle everything
}
