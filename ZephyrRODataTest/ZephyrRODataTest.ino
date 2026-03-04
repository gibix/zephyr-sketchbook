/*
  ZephyrRODataTest

  Tests LLEXT handling of large read-only data in the .rodata section.
  Includes a ~37KB const array (rodata.h) to verify that:
    - The LLEXT loader can allocate and map large rodata regions
    - With CONFIG_LLEXT_RODATA_NO_RELOC=y, rodata stays in flash (no RAM copy)
    - Without the zephyr patch "Skip MPU alignment if userspace disabled",
      a region >32KB rounds to 64KB with 64KB alignment, which can exhaust
      the 128KB LLEXT heap

  Expected output:
    === ZephyrRODataTest started ===
    RANDOM_DATA address: 0x........     (flash address if NO_RELOC, RAM if copied)
    RANDOM_DATA count: 1864
    heartbeat: ability                  (cycles through words each loop)

  If no output at all: the LLEXT module likely failed to load.
  Check with CONFIG_LLEXT_LOG_LEVEL_DBG=y in loader/prj.conf.
*/

#include "rodata.h"

static size_t idx = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000); // wait for USB CDC enumeration

  printk("=== ZephyrRODataTest started ===\n");
  printk("RANDOM_DATA address: 0x%lx\n", (uintptr_t)RANDOM_DATA);
  printk("RANDOM_DATA count: %zu\n", RANDOM_DATA_COUNT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);

  if (idx >= RANDOM_DATA_COUNT) {
    idx = 0;
  }
  printk("heartbeat: %s\n", RANDOM_DATA[idx++]);
}
