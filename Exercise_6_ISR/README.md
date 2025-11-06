![image](https://github.com/mariyahaider100/RTOS-course/blob/b47a7660b4dce92cd18307b98f66bf8d57fb86db/Exercise_6_ISR/Screenshot%202025-11-06%20at%2020.17.50.png)

# ESP32 Timer + Semaphore Rolling Average Demo (Arduino-ESP32 v3.x)

This project reads an **analog input once per second (1 Hz)** using `esp_timer`, notifies a FreeRTOS task via a **binary semaphore**, and displays the **rolling average** of the five most recent readings.

---

## ⚙️ Features

- Utilizes the **`esp_timer` API** (Arduino-ESP32 v3.x) to trigger periodic callbacks.  
- The timer interrupt samples the **ADC** every second.  
- A **FreeRTOS binary semaphore** signals a task to compute and print the **5-sample rolling average** safely between ISR and task context.  

---

## 🧰 Requirements

- **Arduino IDE 2.x** (or **PlatformIO**)  
- **ESP32 Arduino Core** version **3.x or newer**  
- Any **ESP32 development board**  
- **Analog input** connected to the chosen ADC pin (default: `A0`)  

---


## 🚀 Getting Started

1. Open the sketch file:  
   `exercise_l6_isr.ino`  
2. Choose your **ESP32 board** and correct **serial port**.  
3. Upload the code and open the **Serial Monitor** at `115200` baud.  
4. Observe the rolling average values printed once per second.

---

## ⚙️ Configuration Options

- **ADC Pin:**  
  Modify the variable `adc_pin` (default: `A0`) to match an analog-capable pin on your ESP32 board.  

- **Sampling Frequency:**  
  Adjust `period_us` (default: `1,000,000` µs → 1 Hz) to change how often the ADC is sampled.  

- **Task Pinning:**  
  The task runs on `app_cpu` — **core 1** on dual-core ESP32, or **core 0** on single-core variants.  

---

✅ This demo illustrates **real-time ADC sampling**, **interrupt-driven signaling**, and **task synchronization** using **FreeRTOS semaphores** with the **ESP32 timer system**.
