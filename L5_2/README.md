## 🔒 FreeRTOS Mutex Demo (ESP32)

### 🎯 Task Goal

1. Understand how **mutex (mutual exclusion)** works in FreeRTOS.
2. Input an integer from the Serial Monitor to set the  **start value of a shared counter** .
3. Display the counter safely using a mutex so multiple tasks can update it without conflicts.

---

### 🧠 Concept

A **mutex** ensures only one task accesses shared data at a time — preventing data corruption when multiple tasks read/write the same variable.

If one task “takes” the mutex, others must wait until it’s “given” back.

---

### 🧩 Final Code

<pre class="overflow-visible!" data-start="684" data-end="2380"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre! language-cpp"><span><span>#include</span><span></span><span><Arduino.h></span><span>
</span><span>#include</span><span></span><span>"freertos/FreeRTOS.h"</span><span>
</span><span>#include</span><span></span><span>"freertos/semphr.h"</span><span>

</span><span>static</span><span></span><span>int</span><span> shared_counter = </span><span>0</span><span>;
SemaphoreHandle_t mutex;

</span><span>// Task: Increment shared counter safely</span><span>
</span><span>void</span><span></span><span>incTask</span><span>(void</span><span> *parameters) {
  </span><span>int</span><span> local_var;

  </span><span>while</span><span> (</span><span>1</span><span>) {
    </span><span>if</span><span> (</span><span>xSemaphoreTake</span><span>(mutex, portMAX_DELAY) == pdTRUE) {
      local_var = shared_counter;
      local_var++;
      </span><span>vTaskDelay</span><span>(</span><span>pdMS_TO_TICKS</span><span>(</span><span>100</span><span>));  </span><span>// simulate work</span><span>
      shared_counter = local_var;

      Serial.</span><span>printf</span><span>(</span><span>"Counter = %d\n"</span><span>, shared_counter);

      </span><span>xSemaphoreGive</span><span>(mutex);
    }

    </span><span>vTaskDelay</span><span>(</span><span>pdMS_TO_TICKS</span><span>(</span><span>200</span><span>));
  }
}

</span><span>// Task: Read user input to set counter start value</span><span>
</span><span>void</span><span></span><span>inputTask</span><span>(void</span><span> *parameters) {
  </span><span>while</span><span> (</span><span>1</span><span>) {
    </span><span>if</span><span> (Serial.</span><span>available</span><span>() > </span><span>0</span><span>) {
      </span><span>int</span><span> val = Serial.</span><span>parseInt</span><span>();

      </span><span>if</span><span> (</span><span>xSemaphoreTake</span><span>(mutex, portMAX_DELAY) == pdTRUE) {
        shared_counter = val;
        Serial.</span><span>printf</span><span>(</span><span>"Counter reset to %d\n"</span><span>, shared_counter);
        </span><span>xSemaphoreGive</span><span>(mutex);
      }

      </span><span>// Clear remaining input</span><span>
      </span><span>while</span><span> (Serial.</span><span>available</span><span>()) Serial.</span><span>read</span><span>();
    }

    </span><span>vTaskDelay</span><span>(</span><span>pdMS_TO_TICKS</span><span>(</span><span>100</span><span>));
  }
}

</span><span>void</span><span></span><span>setup</span><span>()</span><span> {
  Serial.</span><span>begin</span><span>(</span><span>115200</span><span>);
  </span><span>vTaskDelay</span><span>(</span><span>pdMS_TO_TICKS</span><span>(</span><span>1000</span><span>));
  Serial.</span><span>println</span><span>(</span><span>"\n--- FreeRTOS Mutex Demo ---"</span><span>);
  Serial.</span><span>println</span><span>(</span><span>"Enter a number to reset the counter:"</span><span>);

  </span><span>// Create mutex</span><span>
  mutex = </span><span>xSemaphoreCreateMutex</span><span>();
  </span><span>if</span><span> (mutex == </span><span>NULL</span><span>) {
    Serial.</span><span>println</span><span>(</span><span>"Failed to create mutex!"</span><span>);
    </span><span>while</span><span> (</span><span>1</span><span>);
  }

  </span><span>// Create tasks</span><span>
  </span><span>xTaskCreate</span><span>(incTask, </span><span>"Increment Task 1"</span><span>, </span><span>2048</span><span>, </span><span>NULL</span><span>, </span><span>1</span><span>, </span><span>NULL</span><span>);
  </span><span>xTaskCreate</span><span>(incTask, </span><span>"Increment Task 2"</span><span>, </span><span>2048</span><span>, </span><span>NULL</span><span>, </span><span>1</span><span>, </span><span>NULL</span><span>);
  </span><span>xTaskCreate</span><span>(inputTask, </span><span>"Input Task"</span><span>, </span><span>2048</span><span>, </span><span>NULL</span><span>, </span><span>1</span><span>, </span><span>NULL</span><span>);

  </span><span>vTaskDelete</span><span>(</span><span>NULL</span><span>);  </span><span>// end setup task</span><span>
}

</span><span>void</span><span></span><span>loop</span><span>()</span><span> {
  </span><span>// Not used</span><span>
}
</span></span></code></div></div></pre>

---

### 🧪 Expected Output

<pre class="overflow-visible!" data-start="2410" data-end="2577"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>--- FreeRTOS Mutex Demo ---
Enter a number to reset the counter:
Counter = 1
Counter = 2
Counter = 3
...
> 50
Counter reset to 50
Counter = 51
Counter = 52
...
</span></span></code></div></div></pre>

---

### 🧭 Steps to Test

**In Wokwi / Arduino IDE:**

1. Create new ESP32 (Arduino) project.
2. Paste the code above.
3. Open Serial Monitor (115200 baud).
4. Observe the counter increasing.
5. Enter a number (e.g. `100`) → counter resets safely to that value.

📸 **Screenshots to include:**

* Serial Monitor showing counter increments
* Reset message after entering a value

---

### ✅ Why It Works

* Both increment tasks use the same shared counter.
* Mutex ensures only one task modifies it at a time.
* The input task also uses the same mutex when resetting — keeping all access thread-safe and consistent.
