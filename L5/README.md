## ⏱️ FreeRTOS Software Timer Demo (ESP32)

### 🎯 Task Goal

1. Understand how **FreeRTOS software timers** work — both *one-shot* (non-auto-reload) and  *auto-reload* .
2. Use  **two timers** :
   * **Timer 1:** Blink onboard LED every  **1 second** .
   * **Timer 2:** Send **ADC (A0) readings** to Serial every  **500 ms** .

---

### 🧠 Concept

FreeRTOS software timers let you run functions periodically or once after a set delay — without using dedicated hardware timers or delay loops.

They run in the  **timer service task** , freeing the CPU for other work.

---

### 🧩 Final Code

<pre class="overflow-visible!" data-start="693" data-end="2442"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre! language-cpp"><span><span>#include</span><span></span><span><Arduino.h></span><span>
</span><span>#include</span><span></span><span>"freertos/FreeRTOS.h"</span><span>
</span><span>#include</span><span></span><span>"freertos/timers.h"</span><span>

</span><span>// Onboard LED pin (ESP32 built-in)</span><span>
</span><span>const</span><span></span><span>int</span><span> LED_PIN = </span><span>2</span><span>;
</span><span>const</span><span></span><span>int</span><span> ADC_PIN = A0;

</span><span>// Timer handles</span><span>
</span><span>static</span><span> TimerHandle_t led_timer = </span><span>NULL</span><span>;
</span><span>static</span><span> TimerHandle_t adc_timer = </span><span>NULL</span><span>;

</span><span>// Callback for both timers</span><span>
</span><span>void</span><span></span><span>myTimerCallback</span><span>(TimerHandle_t xTimer)</span><span> {
  </span><span>uint32_t</span><span> timer_id = (</span><span>uint32_t</span><span>)</span><span>pvTimerGetTimerID</span><span>(xTimer);

  </span><span>// LED blink timer</span><span>
  </span><span>if</span><span> (timer_id == </span><span>0</span><span>) {
    </span><span>static</span><span></span><span>bool</span><span> led_state = </span><span>false</span><span>;
    led_state = !led_state;
    </span><span>digitalWrite</span><span>(LED_PIN, led_state);
    Serial.</span><span>printf</span><span>(</span><span>"[LED Timer] LED %s\n"</span><span>, led_state ? </span><span>"ON"</span><span> : </span><span>"OFF"</span><span>);
  }

  </span><span>// ADC send timer</span><span>
  </span><span>if</span><span> (timer_id == </span><span>1</span><span>) {
    </span><span>int</span><span> value = </span><span>analogRead</span><span>(ADC_PIN);
    Serial.</span><span>printf</span><span>(</span><span>"[ADC Timer] ADC value: %d\n"</span><span>, value);
  }
}

</span><span>void</span><span></span><span>setup</span><span>()</span><span> {
  Serial.</span><span>begin</span><span>(</span><span>115200</span><span>);
  </span><span>pinMode</span><span>(LED_PIN, OUTPUT);
  </span><span>pinMode</span><span>(ADC_PIN, INPUT);
  </span><span>vTaskDelay</span><span>(</span><span>pdMS_TO_TICKS</span><span>(</span><span>1000</span><span>));

  Serial.</span><span>println</span><span>(</span><span>"\n--- FreeRTOS Software Timer Demo ---"</span><span>);

  </span><span>// Create timers</span><span>
  led_timer = </span><span>xTimerCreate</span><span>(</span><span>"LED Timer"</span><span>,
                           </span><span>pdMS_TO_TICKS</span><span>(</span><span>1000</span><span>),   </span><span>// 1s</span><span>
                           pdTRUE,                </span><span>// auto-reload</span><span>
                           (</span><span>void</span><span>*)</span><span>0</span><span>,
                           myTimerCallback);

  adc_timer = </span><span>xTimerCreate</span><span>(</span><span>"ADC Timer"</span><span>,
                           </span><span>pdMS_TO_TICKS</span><span>(</span><span>500</span><span>),    </span><span>// 0.5s</span><span>
                           pdTRUE,                </span><span>// auto-reload</span><span>
                           (</span><span>void</span><span>*)</span><span>1</span><span>,
                           myTimerCallback);

  </span><span>if</span><span> (led_timer == </span><span>NULL</span><span> || adc_timer == </span><span>NULL</span><span>) {
    Serial.</span><span>println</span><span>(</span><span>"Failed to create timers!"</span><span>);
    </span><span>while</span><span> (</span><span>1</span><span>);
  }

  </span><span>// Start both timers</span><span>
  </span><span>xTimerStart</span><span>(led_timer, portMAX_DELAY);
  </span><span>xTimerStart</span><span>(adc_timer, portMAX_DELAY);
}

</span><span>void</span><span></span><span>loop</span><span>()</span><span> {
  </span><span>// Not used — timers handle everything</span><span>
}
</span></span></code></div></div></pre>

---

### 🧪 Expected Output

<pre class="overflow-visible!" data-start="2472" data-end="2643"><div class="contain-inline-size rounded-2xl relative bg-token-sidebar-surface-primary"><div class="sticky top-9"><div class="absolute end-0 bottom-0 flex h-9 items-center pe-2"><div class="bg-token-bg-elevated-secondary text-token-text-secondary flex items-center gap-4 rounded-sm px-2 font-sans text-xs"></div></div></div><div class="overflow-y-auto p-4" dir="ltr"><code class="whitespace-pre!"><span><span>---</span><span></span><span>FreeRTOS</span><span></span><span>Software</span><span></span><span>Timer</span><span></span><span>Demo</span><span></span><span>---</span><span>
[</span><span>LED</span><span></span><span>Timer</span><span>] </span><span>LED</span><span></span><span>ON</span><span>
[</span><span>ADC</span><span></span><span>Timer</span><span>] </span><span>ADC value:</span><span></span><span>1823</span><span>
[</span><span>ADC</span><span></span><span>Timer</span><span>] </span><span>ADC value:</span><span></span><span>1850</span><span>
[</span><span>LED</span><span></span><span>Timer</span><span>] </span><span>LED</span><span></span><span>OFF</span><span>
[</span><span>ADC</span><span></span><span>Timer</span><span>] </span><span>ADC value:</span><span></span><span>1809</span><span>
</span><span>...</span><span>
</span></span></code></div></div></pre>

LED blinks once per second, and ADC values print every half second.

---

### 🧭 How to Test

**In Wokwi:**

1. Create a new ESP32 (Arduino) project.
2. Paste this code.
3. Open Serial Monitor → click Run ▶
4. Observe the LED toggle (in simulation) and printed ADC values.

📸 **Screenshots to include:**

* Serial monitor showing both LED and ADC messages
* (Optional) Wokwi circuit view

---

### ✅ Why It Works

* Both timers run in the FreeRTOS timer service.
* **LED Timer:** toggles the built-in LED at 1 Hz.
* **ADC Timer:** reads analog pin every 500 ms.
* Using `xTimerCreate()` + `xTimerStart()` allows independent, precise timing without blocking tasks.
