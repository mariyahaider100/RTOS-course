# Overview

This lab demonstrates FreeRTOS on the ESP32-S3 using Arduino. Three concurrent tasks were implemented:

1. TaskLED (High Priority) – Cycles the onboard RGB LED (GPIO 38) through Red → Green → Blue → Off using fixed-rate timing.

2. TaskSerial (Medium Priority) – Prints system status, including free heap memory, every second.

3. TaskCompute (Low Priority) – Simulates a light CPU workload with an arithmetic loop.

The project explores task priorities, preemption, and timing accuracy using vTaskDelay() and vTaskDelayUntil().


# How to Run

Open ESP32_FreeRTOS_Lab.ino in Arduino IDE.

Compile and upload to ESP32-S3.

Open Serial Monitor at 115200 baud.

Expected Behavior:

RGB LED cycles Red → Green → Blue → Off every 400 ms.

Serial output prints free heap memory once per second:

```bash
[t=1234 ms] freeHeap=45678 bytes
[t=2234 ms] freeHeap=45612 bytes
```
...

# Experiments

1. Priority Swap:
Swapping TaskSerial to priority 1 and TaskCompute to priority 2 caused irregular serial prints due to CPU preemption.

2. Heavier CPU Load:
Increasing TaskCompute loop iterations slowed LED blinking and disrupted its rhythm, showing the impact of CPU-bound tasks on real-time scheduling.

3. Period vs. Execution Time:
Setting TaskLED period to 200 ms with 150 ms ON time results in a 75% duty cycle. If ON time approaches the period, LED OFF time reduces and tasks may miss fixed-rate deadlines.