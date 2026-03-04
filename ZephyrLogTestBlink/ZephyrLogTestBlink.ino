/*
  ZephyrLogTestBlink

  Tests Zephyr logging, printk console output, and fault handling.
  Blinks the built-in LED and prints messages via:
    - printk()    -> goes to zephyr,console (UART or USB CDC)
    - LOG_WRN()   -> goes to Zephyr log backend (UART/USB, needs LOG_BACKEND_UART_AUTOSTART=y)

  The crash() function triggers k_panic() to test the fault dump output.
  Expects CONFIG_FAULT_DUMP=2 and CONFIG_EXCEPTION_STACK_TRACE=y in the
  board .conf for a full stack trace on crash.

  If using USB CDC as console, printk output before USB enumeration is lost.
  Use delay() or Serial.begin() + while(!Serial) to wait for the host.
*/

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(sketch, LOG_LEVEL_WRN);

void crash(void) {
  k_panic();
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000); // wait for USB CDC enumeration
  printk("=== ZephyrLogTestBlink started ===\n");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  printk("heartbeat\n");
  LOG_WRN("log warning test");
  // Uncomment to test fault dump:
  // crash();
}
