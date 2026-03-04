/*
  ZephyrMallocToDeath

  Stress-tests heap allocation by calling malloc(1KB) in a loop until
  it fails. Useful for determining the effective heap size available
  to sketches at runtime.

  Expected output:
    === ZephyrMallocToDeath started ===
    [1] malloc(1024) = 0x20012340
    [2] malloc(1024) = 0x20012740
    ...
    [N] malloc(1024) = (nil) -- heap exhausted after N-1 KB

  The LED blinks fast (50ms) on each allocation. When malloc returns NULL,
  the sketch keeps running but stops allocating (LED still blinks).

  Note: memory is intentionally never freed to find the limit.
  CONFIG_COMMON_LIBC_MALLOC_ARENA_SIZE=-1 uses all remaining RAM for heap.
*/

void setup() {
  Serial.begin(115200);
  while (!Serial);
  pinMode(LED_BUILTIN, OUTPUT);
  printk("=== ZephyrMallocToDeath started ===\n");
}

static int i = 0;
static bool exhausted = false;

void loop() {
  if (!exhausted) {
    i++;
    void *buf = malloc(1024);
    if (buf) {
      printk("[%d] malloc(1024) = %p\n", i, buf);
      Serial.print(i);
      Serial.print(": 0x");
      Serial.println((uint32_t)buf, HEX);
    } else {
      printk("[%d] malloc(1024) = (nil) -- heap exhausted after %d KB\n", i, i - 1);
      Serial.print("Heap exhausted after ");
      Serial.print(i - 1);
      Serial.println(" KB");
      exhausted = true;
    }
  }

  digitalWrite(LED_BUILTIN, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
}
