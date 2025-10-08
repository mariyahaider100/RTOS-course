#include <Arduino.h>

// Use only core 1
#if CONFIG_FREERTOS_UNICORE
    static const BaseType_t app_cpu = 0;
#else
    static const BaseType_t app_cpu = 1;
#endif

// Queue length
static const uint8_t msg_queue_len = 5;
static QueueHandle_t msg_queue;

// TaskReceiver: waits for message and prints it
void TaskReceiver(void *parameter) {
    int item;
    while (1) {
        // Wait up to 100 ticks for message
        if (xQueueReceive(msg_queue, &item, pdMS_TO_TICKS(100)) == pdTRUE) {
        Serial.printf("Received: %d\n", item);
        }
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

// TaskSender: sends numbers to queue
void TaskSender(void *parameter) {
    int num = 0;
    while (1) {
        if (xQueueSend(msg_queue, &num, pdMS_TO_TICKS(10)) != pdTRUE) {
        Serial.println("Queue full!");
        } else {
        Serial.printf("Sent: %d\n", num);
        }
        num++;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void setup() {
    Serial.begin(115200);
    vTaskDelay(pdMS_TO_TICKS(1000));
    Serial.println("\n--- FreeRTOS Queue Demo ---");

    // Create the queue
    msg_queue = xQueueCreate(msg_queue_len, sizeof(int));
    if (msg_queue == NULL) {
        Serial.println("Failed to create queue!");
        while (1);
    }

    // Create sender and receiver tasks
    xTaskCreatePinnedToCore(TaskSender, "TaskSender", 2048, NULL, 1, NULL, app_cpu);
    xTaskCreatePinnedToCore(TaskReceiver, "TaskReceiver", 2048, NULL, 1, NULL, app_cpu);
}

void loop() {
  // Empty (main task deleted)
}
