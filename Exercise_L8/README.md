# 🧠 ESP32 FreeRTOS Deadlock Demo & Solutions

![image](/Exercise%20L8/Screenshot%20from%202025-11-02%2023-59-46.png)

This project showcases how **deadlocks** can occur in **FreeRTOS** running on the **ESP32**, and presents **three effective strategies** to avoid or resolve them:

1. **Timeout-based mutex acquisition**  
2. **Consistent mutex hierarchy (lock order)**  
3. **Global arbitrator (centralized locking control)**  

---


## 🚀 Overview

In multitasking environments, a **deadlock** happens when multiple tasks are blocked indefinitely, each waiting for a resource held by another task.

This demo involves:
- Two FreeRTOS tasks → `Task A` and `Task B`  
- Two mutexes → `mutex_1` and `mutex_2`  
- An optional **global mutex** → `arbitrator`  

Both tasks attempt to acquire the two mutexes, but in opposite order — leading to a **classic deadlock**.  
The program demonstrates three different prevention or recovery methods, which can be selected by defining a mode in the sketch.

---

## ⚙️ Hardware & Software Setup

### 🧩 Hardware
- ESP32 / ESP32-S3 development board  
- USB connection for uploading and monitoring via Serial  

### 💻 Software
- Arduino IDE (version 2.x recommended)  
- ESP32 Board Support Package  
- FreeRTOS (included with the ESP32 SDK)  
- Serial Monitor running at **115200 baud**

---

## 🧪 Deadlock Scenario

In the base version of the code, both tasks compete for two mutexes but in opposite order:

- **Task A** locks `mutex_1`, then tries to lock `mutex_2`.  
- **Task B** locks `mutex_2`, then tries to lock `mutex_1`.  
- Both remain blocked indefinitely → **deadlock occurs!**

```cpp
// Task A
xSemaphoreTake(mutex_1, portMAX_DELAY);
vTaskDelay(1);
xSemaphoreTake(mutex_2, portMAX_DELAY);

// Task B
xSemaphoreTake(mutex_2, portMAX_DELAY);
vTaskDelay(1);
xSemaphoreTake(mutex_1, portMAX_DELAY);
```

---

## 💡 Deadlock Resolution Techniques

### 🕓 1. Timeout-Based Mutex

Add a timeout when acquiring a mutex.  
If a task can’t obtain the second mutex within the given time, it releases the first one, waits, and retries later.

```cpp
if (xSemaphoreTake(mutex_1, mutex_timeout) == pdTRUE) {
    if (xSemaphoreTake(mutex_2, mutex_timeout) == pdTRUE) {
        // Perform work safely
        xSemaphoreGive(mutex_2);
        xSemaphoreGive(mutex_1);
    } else {
        // Retry later
        xSemaphoreGive(mutex_1);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
```

✅ **Pros:** Simple and non-blocking  
⚠️ **Cons:** Requires careful timeout tuning and retry logic

---

### 🧱 2. Mutex Hierarchy (Consistent Lock Order)

All tasks acquire mutexes in the **same global order**, such as always locking `mutex_1` before `mutex_2`.

```cpp
// Both tasks follow this rule
xSemaphoreTake(mutex_1, portMAX_DELAY);
xSemaphoreTake(mutex_2, portMAX_DELAY);
```

✅ **Pros:** Completely eliminates deadlock  
⚠️ **Cons:** Requires strict adherence across all code sections

---

### 🔐 3. Global Arbitrator (Central Mutex)

Use a **global mutex** that controls access to all other mutexes.  
Tasks must first acquire the arbitrator before taking individual mutexes.

```cpp
xSemaphoreTake(arbitrator, portMAX_DELAY);
xSemaphoreTake(mutex_1, portMAX_DELAY);
xSemaphoreTake(mutex_2, portMAX_DELAY);
xSemaphoreGive(arbitrator);
```

✅ **Pros:** Very safe and easy to reason about  
⚠️ **Cons:** Slightly limits parallelism since all resource access is serialized

---

## 🧭 How to Run the Example

### Step 1️⃣ — Clone the Repository


### Step 2️⃣ — Open in Arduino IDE

1. Open the sketch file `ESP32_Deadlock_Demo.ino`  
2. Select your **board** → `ESP32 Dev Module`  
3. Set **baud rate** to `115200`

### Step 3️⃣ — Select a Resolution Method

At the beginning of the code, uncomment **one** of the following options:

```cpp
//#define MODE_TIMEOUT
//#define MODE_HIERARCHY
#define MODE_ARBITRATOR
```

### Step 4️⃣ — Upload and Observe

Upload the code and open the **Serial Monitor** at `115200 baud`.  
You’ll see log messages showing which task holds which mutex and how the chosen method prevents deadlock:

```
Task A took arbitrator
Task A took mutex 1
Task A took mutex 2
Task A released arbitrator
Task A doing some work
Task A sleeping
Task B took arbitrator
Task B took mutex 1
Task B took mutex 2
Task B released arbitrator
Task B doing some work
Task B sleeping
```

---

✅ This demo clearly illustrates **how deadlocks form in concurrent systems** and how different **FreeRTOS synchronization strategies** can effectively prevent or recover from them.
