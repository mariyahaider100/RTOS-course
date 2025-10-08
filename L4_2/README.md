## 📨 FreeRTOS Queue Demo (ESP32)

### 🎯 Task Goal

Create two tasks:

* **TaskSender:** sends a message (value or structure) to a queue every few seconds.
* **TaskReceiver:** waits for a message in the queue and displays it once received.

This exercise demonstrates inter-task communication using a **FreeRTOS queue** on the ESP32.

---

### 🧠 Issue & Concept

Without queues, tasks might interfere with each other when sharing data.

FreeRTOS queues solve this by safely passing messages between tasks — no shared memory needed, no race conditions.

---

### 🧩 Final Code

<pre class="overflow-visible!" data-start="721" data-end="2177"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre! language-cpp"><span><span>#include</span><span></span><span><Arduino.h></span><span>

</span><span>// Use only core 1</span><span>
</span><span>#if</span><span> CONFIG_FREERTOS_UNICORE
  </span><span>static</span><span></span><span>const</span><span> BaseType_t app_cpu = </span><span>0</span><span>;
</span><span>#else</span><span>
  </span><span>static</span><span></span><span>const</span><span> BaseType_t app_cpu = </span><span>1</span><span>;
</span><span>#endif</span><span>

</span><span>// Queue length</span><span>
</span><span>static</span><span></span><span>const</span><span></span><span>uint8_t</span><span> msg_queue_len = </span><span>5</span><span>;
</span><span>static</span><span> QueueHandle_t msg_queue;

</span><span>// TaskReceiver: waits for message and prints it</span><span>
</span><span>void</span><span></span><span>TaskReceiver</span><span>(void</span><span> *parameter) {
  </span><span>int</span><span> item;
  </span><span>while</span><span> (</span><span>1</span><span>) {
    </span><span>// Wait up to 100 ticks for message</span><span>
    </span><span>if</span><span> (</span><span>xQueueReceive</span><span>(msg_queue, &item, </span><span>pdMS_TO_TICKS</span><span>(</span><span>100</span><span>)) == pdTRUE) {
      Serial.</span><span>printf</span><span>(</span><span>"Received: %d\n"</span><span>, item);
    }
    </span><span>vTaskDelay</span><span>(</span><span>pdMS_TO_TICKS</span><span>(</span><span>200</span><span>));
  }
}

</span><span>// TaskSender: sends numbers to queue</span><span>
</span><span>void</span><span></span><span>TaskSender</span><span>(void</span><span> *parameter) {
  </span><span>int</span><span> num = </span><span>0</span><span>;
  </span><span>while</span><span> (</span><span>1</span><span>) {
    </span><span>if</span><span> (</span><span>xQueueSend</span><span>(msg_queue, &num, </span><span>pdMS_TO_TICKS</span><span>(</span><span>10</span><span>)) != pdTRUE) {
      Serial.</span><span>println</span><span>(</span><span>"Queue full!"</span><span>);
    } </span><span>else</span><span> {
      Serial.</span><span>printf</span><span>(</span><span>"Sent: %d\n"</span><span>, num);
    }
    num++;
    </span><span>vTaskDelay</span><span>(</span><span>pdMS_TO_TICKS</span><span>(</span><span>1000</span><span>));
  }
}

</span><span>void</span><span></span><span>setup</span><span>()</span><span> {
  Serial.</span><span>begin</span><span>(</span><span>115200</span><span>);
  </span><span>vTaskDelay</span><span>(</span><span>pdMS_TO_TICKS</span><span>(</span><span>1000</span><span>));
  Serial.</span><span>println</span><span>(</span><span>"\n--- FreeRTOS Queue Demo ---"</span><span>);

  </span><span>// Create the queue</span><span>
  msg_queue = </span><span>xQueueCreate</span><span>(msg_queue_len, </span><span>sizeof</span><span>(</span><span>int</span><span>));
  </span><span>if</span><span> (msg_queue == </span><span>NULL</span><span>) {
    Serial.</span><span>println</span><span>(</span><span>"Failed to create queue!"</span><span>);
    </span><span>while</span><span> (</span><span>1</span><span>);
  }

  </span><span>// Create sender and receiver tasks</span><span>
  </span><span>xTaskCreatePinnedToCore</span><span>(TaskSender, </span><span>"TaskSender"</span><span>, </span><span>2048</span><span>, </span><span>NULL</span><span>, </span><span>1</span><span>, </span><span>NULL</span><span>, app_cpu);
  </span><span>xTaskCreatePinnedToCore</span><span>(TaskReceiver, </span><span>"TaskReceiver"</span><span>, </span><span>2048</span><span>, </span><span>NULL</span><span>, </span><span>1</span><span>, </span><span>NULL</span><span>, app_cpu);
}

</span><span>void</span><span></span><span>loop</span><span>()</span><span> {
  </span><span>// Empty (main task deleted)</span><span>
}
</span></span></code></div></div></pre>

---

### 🧪 Expected Output

<pre class="overflow-visible!" data-start="2207" data-end="2306"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>--- FreeRTOS Queue Demo ---
</span><span>Sent: 0</span><span>
</span><span>Received: 0</span><span>
</span><span>Sent: 1</span><span>
</span><span>Received: 1</span><span>
</span><span>Sent: 2</span><span>
</span><span>Received: 2</span><span>
...
</span></span></code></div></div></pre>

---

### 🧭 Wokwi / Hardware Steps

1. Go to [wokwi.com](https://wokwi.com/) → create new **ESP32 (Arduino)** project.
2. Replace default sketch with this code.
3. Run ▶ → open Serial Monitor.
4. Confirm alternating *Sent* and *Received* messages.

📸 **Screenshot to attach:** Serial monitor showing both tasks communicating.

---

### ✅ Why It Works

* **TaskSender** adds messages to the queue every second.
* **TaskReceiver** waits and prints each message it gets.
* The queue ensures synchronization and safe data passing between tasks.
