---
# 🔦 ESP32 FreeRTOS Binary Semaphore Example

This project shows how to **safely share a stack-based variable** between tasks on an **ESP32** using a **binary semaphore** for synchronization.

---

## 🧠 Overview of the Code

When creating a FreeRTOS task and passing a **local (stack) variable**—for example, one declared in `setup()`—there’s a danger that its memory could be overwritten before the new task copies it.

To handle this safely, a **binary semaphore** is used to coordinate between the main function (`setup`) and the secondary task (`blinkLED`):

1. The main program (`setup`) reads a user-defined delay value through Serial.  
2. It creates a **binary semaphore** (`bin_sem`).  
3. Then it starts the `blinkLED` task and sends the delay value as an argument.  
4. The new task immediately **copies the received value** into a local variable and then **gives (releases)** the semaphore to notify completion.  
5. Meanwhile, the main program **waits (takes)** the semaphore, ensuring that the variable is safely copied before the original goes out of scope.

---

## 💡 Key Takeaways

* Binary semaphores are ideal for **task-to-task synchronization**.  
* They ensure **safe access to temporary or stack-based data**.  
* By waiting for the semaphore signal, the main function confirms the new task has already **secured its parameter** before moving forward.

---

## 🔧 Hardware Setup

* **Board:** ESP32 or ESP32-S3  
* **LED Pin:** Built-in (`LED_BUILTIN`)  
* **Serial Speed:** 115200 baud  

After uploading the sketch, open the **Serial Monitor**, type a delay value (for example `500`), and watch the built-in LED blink at that interval.

---

## 📜 Sample Output

```
---FreeRTOS Semaphore Demo---
Enter delay (milliseconds):
Sending: 500
Received: 500
Done!
```

The LED will then blink every 500 milliseconds repeatedly.

---

## 🧭 Usage Instructions

### Step 1️⃣ — Clone the Repository
