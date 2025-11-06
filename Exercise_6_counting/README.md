![image](/Exercise%20L6%20Counting/Screenshot%20from%202025-11-03%2000-31-21.png)

# FreeRTOS Counting Semaphore Example (ESP32)

This project demonstrates the use of **FreeRTOS counting semaphores** on the **ESP32**, divided into two main parts:

- **Part 1:** Create and coordinate five tasks using a counting semaphore, ensuring that the main task waits until all child tasks have safely handled a shared variable.  
- **Part 2:** Ask the user for a sentence, split it into individual words, and dynamically create one task per word, where each task receives a separate `Message`.

---

## ✨ Features

- Employs a **counting semaphore** as a synchronization barrier to confirm that each task has successfully copied its parameters.  
- Ensures **safe parameter transfer** between the main task and child tasks:  
  - **Part 1:** Each of the five tasks quickly copies the same stack-local `Message`, then signals completion using the semaphore.  
  - **Part 2:** Each word-task receives a **heap-allocated** `Message*`, copies the contents locally, frees the pointer, and signals the semaphore.  
- Every task prints its message and **self-deletes** after execution.

---

## 🧠 Hardware / Software Requirements

- **Board:** ESP32 Development Board (Arduino Core)  
- **Interface:** Serial Monitor at 115200 baud  
- **Framework:** FreeRTOS (built into the ESP32 Arduino core)

---

## ⚙️ How It Works

### Message Structure
- `msg.body` → A character array (`char[20]`) storing the message or word (limited to 19 characters + null terminator).  
- `msg.len` → Stores the length of the string.

---

### 🧩 Part 1 – Fixed 5 Tasks

- A **counting semaphore** is created with an initial value of `0` and a maximum count of `5`.  
- The program spawns **five tasks**, all referencing the same **stack-based `Message`**.  
- Each task copies the data to a local variable and then **gives (releases)** the semaphore.  
- The main (creator) task **takes** the semaphore five times, ensuring all tasks have completed copying before proceeding.

---

### 🧱 Part 2 – Dynamic Tasks per Word

- The program reads a full **sentence from Serial input**.  
- It splits the sentence into **individual words**, spawning one **task per word**.  
- Each task receives a **unique heap-allocated `Message*`** that contains its assigned word.  
- Inside the task:
  - The word is copied to a local variable.  
  - The allocated memory is freed.  
  - The task **signals the semaphore**, prints the message, and then deletes itself.

---

✅ This example clearly demonstrates **multi-task synchronization and safe parameter handling** using **FreeRTOS counting semaphores** on the ESP32.
