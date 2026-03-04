/*
  ZephyrSizeLimitTest

  Validates that the compile-time size check (maximum_data_size in boards.txt)
  correctly enforces the LLEXT heap limit for each board.

  Uses board-specific ARDUINO_* defines to size a payload array just under
  the LLEXT heap limit. If it compiles, the limit is correct. If it fails
  with "data section exceeds available space", the limit in boards.txt is
  too small (or the sketch overhead grew).

  To test rejection, increase the payload by changing MARGIN to a negative
  value (e.g., -2048), which will push past the limit.

  Board limits (CONFIG_LLEXT_HEAP_SIZE * 1024):
    niclasense:                  15 KB (15360)
    ek_ra8d1, frdm_mcxn947,
    frdm_rw612:                  32 KB (32768)
    nano_matter:                 64 KB (65536)
    giga, nano33ble, portentah7,
    portentac33, opta, unoq:    128 KB (131072)
*/

// LLEXT heap size per board (bytes)
#if defined(ARDUINO_GIGA) || defined(ARDUINO_NANO33BLE) || \
    defined(ARDUINO_PORTENTA_H7_M7) || defined(ARDUINO_PORTENTA_C33) || \
    defined(ARDUINO_OPTA) || defined(ARDUINO_UNO_Q)
  #define LLEXT_HEAP_BYTES 131072
#elif defined(ARDUINO_ARDUINO_NANO_MATTER)
  #define LLEXT_HEAP_BYTES 65536
#elif defined(ARDUINO_EK_RA8D1) || defined(ARDUINO_FRDM_MCXN947) || \
      defined(ARDUINO_FRDM_RW612)
  #define LLEXT_HEAP_BYTES 32768
#elif defined(ARDUINO_NICLA_SENSE_ME)
  #define LLEXT_HEAP_BYTES 15360
#else
  #error "Unknown board — add its LLEXT heap size here"
#endif

// Overhead: .text + .rodata + .data + .bss excluding the payload array.
// Measured at ~4000 bytes. Use 5120 (5KB) for safety margin.
#define OVERHEAD 5120

// Margin below the limit. Set to a negative value (e.g. -2048) to test
// that the size check correctly rejects oversized sketches.
#define MARGIN 2048

#define PAYLOAD_BYTES (LLEXT_HEAP_BYTES - OVERHEAD - MARGIN)

#if PAYLOAD_BYTES <= 0
  #error "Board heap too small for this test (limit < overhead + margin)"
#endif

// Volatile prevents the compiler from optimizing away the array.
// .bss section (zero-initialized, allocated on LLEXT heap).
static volatile uint8_t payload[PAYLOAD_BYTES];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);

  payload[0] = 0xAA;
  payload[PAYLOAD_BYTES - 1] = 0x55;

  printk("=== ZephyrSizeLimitTest ===\n");
  printk("LLEXT heap: %d bytes\n", LLEXT_HEAP_BYTES);
  printk("Payload:    %d bytes at %p\n", PAYLOAD_BYTES, (void *)payload);
  printk("Headroom:   %d bytes\n", LLEXT_HEAP_BYTES - PAYLOAD_BYTES);
  printk("First: 0x%02x, Last: 0x%02x\n", payload[0], payload[PAYLOAD_BYTES - 1]);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
