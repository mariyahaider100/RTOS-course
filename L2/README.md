
# FreeRTOS Rate Monotonic Scheduling on ESP32-S3

## Overview

This experiment demonstrates **real-time task scheduling** on the **ESP32-S3** using  **FreeRTOS** .

Three LEDs represent periodic tasks with different execution times and priorities following the **Rate Monotonic Scheduling (RMS)** principle.

* **LED1 (Red)** – Task1, period 500 ms, execution 200 ms, lowest priority
* **LED2 (Yellow)** – Task2, period 200 ms, execution 100 ms, highest priority
* **LED3 (Green)** – Task3, period 1000 ms, execution 300 ms, medium priority

Each LED’s ON time simulates task computation, and OFF time represents waiting until the next period.

---

## How It Works

* Tasks are created using `xTaskCreate()` with priorities 1–3.
* `vTaskDelay()` simulates computation (LED ON).
* `vTaskDelayUntil()` ensures precise periodic timing.
* The FreeRTOS scheduler preempts lower-priority tasks when higher-priority ones are ready.

---

## Expected Behavior

* The **yellow LED (Task2)** blinks fastest, preempting the others.
* The **red LED (Task1)** blinks at a medium rate.
* The **green LED (Task3)** blinks slowest.
* The blinking pattern visually shows  **task scheduling and preemption** .

---

## Key Learning Points

* Understanding **Rate Monotonic Scheduling (RMS)**
* Using `vTaskDelayUntil()` for periodic task control
* Observing **preemption** and **task priority effects** through LED timing
* Real-time systems rely on precise scheduling to meet deadlines

---

## Hardware

| LED | Color  | GPIO   | Note  |
| --- | ------ | ------ | ----- |
| D1  | Red    | GPIO 2 | Task1 |
| D2  | Yellow | GPIO 4 | Task2 |
| D3  | Green  | GPIO 5 | Task3 |

*(Use 220 Ω resistors for each LED on real hardware.)*

---

## Conclusion

The project successfully illustrates **FreeRTOS task scheduling** on ESP32-S3.

Higher-priority tasks (shorter periods) preempt lower ones, confirming **Rate Monotonic theory** and demonstrating real-time task management in embedded systems.
