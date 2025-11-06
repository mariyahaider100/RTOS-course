# ⚙️ ESP32 Multicore FreeRTOS Demonstration

This project explores how FreeRTOS tasks execute across multiple cores on the **ESP32-S3**, showing how **task pinning** and **core affinity** affect scheduling and performance.  
Two continuously running tasks ("hog tasks") are created to observe which CPU core handles them, both with and without explicit pinning.

---

## 🧠 Purpose of the Demo

- Launches two FreeRTOS tasks:
  - **Task L** → Low priority  
  - **Task H** → High priority  
- Each task prints its running core using `xPortGetCoreID()`.  
- Both simulate CPU-intensive work with a busy-wait loop (`hog_delay`).  
- Demonstrates how **core pinning** and `tskNO_AFFINITY` influence task scheduling across dual cores.

---

## 🧩 Hardware / Software Requirements

- **Board:** ESP32, ESP32-S3, or any dual-core variant  
- **Environment:** Arduino IDE or ESP-IDF (Arduino-style sketch)  
- **Serial Monitor:** 115200 baud  

---

## 🚀 Steps to Run

1. Upload the demo sketch to your ESP32 board.  
2. Open **Serial Monitor** at `115200`.  
3. Observe printed messages while testing three modes:  
   - Default (using `tskNO_AFFINITY`)  
   - Explicit pinning: Task L → Core 0, Task H → Core 1  
   - No affinity: both tasks allowed to migrate freely between cores  

---

## 🧩 Core Code Snippet

Here’s the key section of the demo showing task creation and core pinning options:

```cpp
// Core definitions
static const BaseType_t pro_cpu = 0;
static const BaseType_t app_cpu = 1;
static const TickType_t time_hog = 200;  // ms

// Simulated CPU work
static void hog_delay(uint32_t ms) {
  for (uint32_t i = 0; i < ms; i++) {
    for (uint32_t j = 0; j < 40000; j++) asm("nop");
  }
}

void doTaskL(void *parameters) {
  char str[40];
  while (1) {
    sprintf(str, "Task L, Core %i\r\n", xPortGetCoreID());
    Serial.print(str);
    hog_delay(time_hog);
  }
}

void doTaskH(void *parameters) {
  char str[40];
  while (1) {
    sprintf(str, "Task H, Core %i\r\n", xPortGetCoreID());
    Serial.print(str);
    hog_delay(time_hog);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Multicore Demo---");

  // Test different configurations:
  // Pinning Example (Task L on core 0, Task H on core 1):
  // xTaskCreatePinnedToCore(doTaskL, "Task L", 2048, NULL, 1, NULL, pro_cpu);
  // xTaskCreatePinnedToCore(doTaskH, "Task H", 2048, NULL, 2, NULL, app_cpu);

  // No affinity example:
  // xTaskCreatePinnedToCore(doTaskL, "Task L", 2048, NULL, 1, NULL, tskNO_AFFINITY);
  // xTaskCreatePinnedToCore(doTaskH, "Task H", 2048, NULL, 2, NULL, tskNO_AFFINITY);

  // Currently active configuration:
  xTaskCreatePinnedToCore(doTaskL, "Task L", 2048, NULL, 1, NULL, tskNO_AFFINITY);
  xTaskCreatePinnedToCore(doTaskH, "Task H", 2048, NULL, 2, NULL, tskNO_AFFINITY);

  vTaskDelete(NULL);
}

void loop() {
  // Unused in FreeRTOS context
}
```

---

## 🔧 Modifying Core Affinity

- **Pin both tasks to specific cores**
  ```cpp
  xTaskCreatePinnedToCore(doTaskL, "Task L", 2048, NULL, 1, NULL, pro_cpu); // Core 0
  xTaskCreatePinnedToCore(doTaskH, "Task H", 2048, NULL, 2, NULL, app_cpu); // Core 1
  ```

- **Allow tasks to float between cores**
  ```cpp
  xTaskCreatePinnedToCore(doTaskL, "Task L", 2048, NULL, 1, NULL, tskNO_AFFINITY);
  xTaskCreatePinnedToCore(doTaskH, "Task H", 2048, NULL, 2, NULL, tskNO_AFFINITY);
  ```

---

## 📊 Observed Serial Output

### Case 1 — Tasks Pinned (L → Core 0, H → Core 1)

```
---FreeRTOS Multicore Demo---
Task L, Core 0
Task H, Core 1
Task H, Core 1
Task L, Core 0
Task H, Core 1
Task L, Core 0
...
```

![image](/Exercise%20L9/Core_0_1.png)

🔹 Each task consistently runs on its assigned core, confirming correct pinning and parallel execution.  

---

### Case 2 — No Affinity (`tskNO_AFFINITY`)

```
---FreeRTOS Multicore Demo---
Task L, Core 1
Task H, Core 1
Task H, Core 1
Task L, Core 0
Task H, Core 1
Task L, Core 0
...
```

![image](/Exercise%20L9/Core_tskNO_AFFINITY.png)

🔹 Tasks begin on Core 1 (the default for Arduino `setup()`), but the scheduler later migrates one to Core 0, showing that FreeRTOS can dynamically balance load when no affinity is set.

---

## 🧩 Analysis & Observations

- The function `xPortGetCoreID()` accurately reports the core where the task is executing.  
- With **explicit pinning**, each task remains fixed to its designated core — enabling **true parallel execution**.  
- With **no affinity**, tasks start on the same core but may migrate as the scheduler balances load.  
- Multiple consecutive prints (e.g., “Task H, Core 1”) occur because the **higher-priority task** may preempt the lower one multiple times before it yields.  
- The **migration behavior** depends on:  
  - Task creation core (typically Core 1 for Arduino setup).  
  - Scheduler load balancing and available CPU time.  

No crashes or resets were observed — only different scheduling patterns based on task affinity configuration.

---

✅ This demo provides a clear view of **how FreeRTOS manages dual-core scheduling**, **task affinity**, and **priority-based execution** on the **ESP32 platform**.
